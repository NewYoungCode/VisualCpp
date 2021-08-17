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
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);//��ʼ��gdi+
		}
	}
	inline void CleanUp() {
		if (gdiplusToken) {
			Gdiplus::GdiplusShutdown(gdiplusToken); //�ر�gdi+
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
	//����ͼƬ
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
		//  ����λͼ�ļ���ͷ
		BITMAPFILEHEADER bmfh;
		//  ����λͼ��Ϣ��ͷ
		BITMAPINFOHEADER bmih;

		//  ��ɫ�峤��
		int nColorLen = 0;
		//  ��ɫ���С
		DWORD dwRgbQuadSize = 0;
		//  λͼ��С
		DWORD dwBmSize = 0;
		//  �����ڴ��ָ��
		HGLOBAL    hMem = NULL;

		LPBITMAPINFOHEADER     lpbi;

		BITMAP bm;
		HDC hDC;

		HANDLE hFile = NULL;

		DWORD dwWritten;

		GetObject(hBm, sizeof(BITMAP), &bm);

		bmih.biSize = sizeof(BITMAPINFOHEADER);    // ���ṹ��ռ���ֽ�
		bmih.biWidth = bm.bmWidth;            // λͼ��
		bmih.biHeight = bm.bmHeight;            // λͼ��
		bmih.biPlanes = 1;
		bmih.biBitCount = bm.bmBitsPixel;    // ÿһͼ�ص�λ��
		bmih.biCompression = BI_RGB;            // ��ѹ��
		bmih.biSizeImage = 0;  //  λͼ��С
		bmih.biXPelsPerMeter = 0;
		bmih.biYPelsPerMeter = 0;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;

		//  ����λͼͼ����������С 
		dwBmSize = 4 * ((bm.bmWidth * bmih.biBitCount + 31) / 32) * bm.bmHeight;

		//  ���ͼ��λ <= 8bit�����е�ɫ��
		if (bmih.biBitCount <= 8)
		{
			nColorLen = (1 << bm.bmBitsPixel);
		}

		//  �����ɫ���С
		dwRgbQuadSize = nColorLen * sizeof(RGBQUAD);

		//  �����ڴ�
		hMem = GlobalAlloc(GHND, dwBmSize + dwRgbQuadSize + sizeof(BITMAPINFOHEADER));

		if (NULL == hMem)
		{
			return FALSE;
		}

		//  �����ڴ�
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hMem);

		//  ��bmih�е�����д�������ڴ���
		*lpbi = bmih;
		hDC = GetDC(NULL);
		//  ��λͼ�е�������bits����ʽ����lpData�С�
		GetDIBits(hDC, hBm, 0, (DWORD)bmih.biHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize, (BITMAPINFO *)lpbi, (DWORD)DIB_RGB_COLORS);

		bmfh.bfType = 0x4D42;  // λͼ�ļ����ͣ�BM
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize + dwBmSize;  // λͼ��С
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize;  // λͼ�������ļ�ͷ����ƫ����

		//  �����������д���ļ���
		hFile = CreateFile(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return FALSE;
		}
		//  д��λͼ�ļ�ͷ
		WriteFile(hFile, (LPSTR)&bmfh, sizeof(BITMAPFILEHEADER), (DWORD *)&dwWritten, NULL);
		//  д��λͼ����
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
