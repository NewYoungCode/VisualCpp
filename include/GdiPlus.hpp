#pragma once
#include <comdef.h>
#include <GdiPlus.h>
#include <gdiplusimagecodec.h>
#include <gdiplusimaging.h>
#include <gdiplusimageattributes.h>
#pragma comment(lib, "gdiplus.lib")

namespace Gdiplus {
	typedef enum : char {
		Jpeg,
		Bmp,
		Png
	}ImageFormat;
	extern ULONG_PTR gdiplusToken;
	void Init();
	void CleanUp();
	bool CutBitmap(Gdiplus::Bitmap & bitmap, const Gdiplus::Rect  & rect, Gdiplus::Bitmap  & outBitmap);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	bool SaveAs(Gdiplus::Image* image, const std::string& out_filename, ImageFormat imgFormat);
	BOOL __SaveBitmapToFile__(LPCTSTR lpszFilePath, HBITMAP hBm);
	bool CutWindowBitmap(HWND hwnd, const std::string&filename);
}

namespace Gdiplus {
	ULONG_PTR gdiplusToken= NULL;
	inline void Init() {
		if (gdiplusToken == NULL) {
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);//初始化gdi+
		}
	}
	inline void CleanUp() {
		if (gdiplusToken) {
			Gdiplus::GdiplusShutdown(gdiplusToken); //关闭gdi+
		}
	}
	inline bool CutBitmap(Gdiplus::Bitmap & bitmap, const Gdiplus::Rect  & rect, Gdiplus::Bitmap  & outBitmap) {
		try
		{
			for (int y = rect.Y; y < rect.Y + rect.Height; y++)
			{
				for (int x = rect.X; x < rect.X + rect.Width; x++)
				{
					Gdiplus::Color r;
					bitmap.GetPixel(x, y, &r);
					outBitmap.SetPixel(x - rect.X, y - rect.Y, r);
				}
			}
			return true;
		}
		catch (const std::exception& e)
		{
			printf("%s\n", e.what());
			return false;
		}
	}
	inline int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes
		Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
			return -1;  // Failure

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL)
			return -1;  // Failure
		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}
		}

		free(pImageCodecInfo);
		return -1;  // Failure
	}
	//保存图片
	inline bool SaveAs(Gdiplus::Image* image, const std::string& out_filename, ImageFormat imgFormat) {
		int bytes = ::MultiByteToWideChar(CP_ACP, 0, out_filename.c_str(), out_filename.size(), NULL, 0);
		std::wstring wstrCmd;
		wstrCmd.resize(bytes);
		bytes = ::MultiByteToWideChar(CP_ACP, 0, out_filename.c_str(), out_filename.size(), const_cast<wchar_t*>(wstrCmd.c_str()), wstrCmd.size());
		CLSID clsid;
		wchar_t buf[128]{ 0 };
		if (imgFormat == ImageFormat::Jpeg) {
			wsprintfW(buf, L"%s", L"image/jpeg");
		}
		if (imgFormat == ImageFormat::Bmp) {
			wsprintfW(buf, L"%s", L"image/bmp");
		}
		if (imgFormat == ImageFormat::Png) {
			wsprintfW(buf, L"%s", L"image/png");
		}
		int r = Gdiplus::GetEncoderClsid(buf, &clsid);
		if (image->Save(wstrCmd.c_str(), &clsid) == 0) {
			return true;
		}
		return false;
	}
	inline BOOL __SaveBitmapToFile__(LPCTSTR lpszFilePath, HBITMAP hBm)
	{
		//  定义位图文件表头
		BITMAPFILEHEADER bmfh;
		//  定义位图信息表头
		BITMAPINFOHEADER bmih;

		//  调色板长度
		int nColorLen = 0;
		//  调色表大小
		DWORD dwRgbQuadSize = 0;
		//  位图大小
		DWORD dwBmSize = 0;
		//  分配内存的指针
		HGLOBAL    hMem = NULL;

		LPBITMAPINFOHEADER     lpbi;

		BITMAP bm;
		HDC hDC;

		HANDLE hFile = NULL;

		DWORD dwWritten;

		GetObject(hBm, sizeof(BITMAP), &bm);

		bmih.biSize = sizeof(BITMAPINFOHEADER);    // 本结构所占的字节
		bmih.biWidth = bm.bmWidth;            // 位图宽
		bmih.biHeight = bm.bmHeight;            // 位图高
		bmih.biPlanes = 1;
		bmih.biBitCount = bm.bmBitsPixel;    // 每一图素的位数
		bmih.biCompression = BI_RGB;            // 不压缩
		bmih.biSizeImage = 0;  //  位图大小
		bmih.biXPelsPerMeter = 0;
		bmih.biYPelsPerMeter = 0;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;

		//  计算位图图素数据区大小 
		dwBmSize = 4 * ((bm.bmWidth * bmih.biBitCount + 31) / 32) * bm.bmHeight;

		//  如果图素位 <= 8bit，则有调色板
		if (bmih.biBitCount <= 8)
		{
			nColorLen = (1 << bm.bmBitsPixel);
		}

		//  计算调色板大小
		dwRgbQuadSize = nColorLen * sizeof(RGBQUAD);

		//  分配内存
		hMem = GlobalAlloc(GHND, dwBmSize + dwRgbQuadSize + sizeof(BITMAPINFOHEADER));

		if (NULL == hMem)
		{
			return FALSE;
		}

		//  锁定内存
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hMem);

		//  将bmih中的内容写入分配的内存中
		*lpbi = bmih;
		hDC = GetDC(NULL);
		//  将位图中的数据以bits的形式放入lpData中。
		GetDIBits(hDC, hBm, 0, (DWORD)bmih.biHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize, (BITMAPINFO *)lpbi, (DWORD)DIB_RGB_COLORS);

		bmfh.bfType = 0x4D42;  // 位图文件类型：BM
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize + dwBmSize;  // 位图大小
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize;  // 位图数据与文件头部的偏移量

		//  把上面的数据写入文件中
		hFile = CreateFile(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return FALSE;
		}
		//  写入位图文件头
		WriteFile(hFile, (LPSTR)&bmfh, sizeof(BITMAPFILEHEADER), (DWORD *)&dwWritten, NULL);
		//  写入位图数据
		WriteFile(hFile, (LPBITMAPINFOHEADER)lpbi, bmfh.bfSize - sizeof(BITMAPFILEHEADER), (DWORD *)&dwWritten, NULL);
		GlobalFree(hMem);
		CloseHandle(hFile);
		return TRUE;
	}
	inline bool CutWindowBitmap(HWND hwnd, const std::string&filename) {
		RECT rect;
		::GetWindowRect(hwnd, &rect);
		int x = rect.left, y = rect.top, width = rect.right - rect.left, height = rect.bottom - rect.top;
		HDC         hdc;
		PAINTSTRUCT ps;
		HBITMAP        hBitmap;
		HDC            hdcMem;
		hdc = GetDC(NULL);
		hBitmap = ::CreateCompatibleBitmap(hdc, width, height);
		hdcMem = ::CreateCompatibleDC(hdc);
		::SelectObject(hdcMem, hBitmap);
		::BitBlt(hdcMem, 0, 0, width, height, hdc, x, y, SRCCOPY);
		bool bResult = __SaveBitmapToFile__(AUTOTEXT(filename), hBitmap);
		ReleaseDC(hwnd, hdcMem);
		ReleaseDC(hwnd, hdc);
		return bResult;
	}
}
