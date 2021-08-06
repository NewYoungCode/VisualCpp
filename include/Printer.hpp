#pragma once
#include <atlimage.h>
#include <Winspool.h>
#pragma comment(lib, "winspool.lib")
namespace Printer {
	typedef enum :char
	{
		A4,
		K16
	}PageSize;
	bool PrinterIamge16K(char* pPrinterName, char* pFilePath);
	bool PrinterImageA4(char* strPrinterName, char* strReportPath);
	//寻找计算机中的打印机
	size_t FindPrinters(std::vector<std::string>& allPrinter);
	//打印一张图片
	bool PrinterImage(const std::string & printer, const std::string & imageFile, PageSize pageSize);
}

namespace Printer {
	inline bool PrinterIamge16K(char* pPrinterName, char* pFilePath)
	{
		bool bResult = false;
		HANDLE     hPrinter;
		PDEVMODE  pDevmode = NULL;
		PRINTER_DEFAULTS pd = { NULL, NULL, PRINTER_ACCESS_USE };
		if (OpenPrinter(AUTOTEXT(pPrinterName), &hPrinter, &pd))
		{
			//获取默认打印机DEVMODE
			LONG nSize = DocumentProperties(NULL, hPrinter, AUTOTEXT(pPrinterName), NULL, NULL, 0);
			if (nSize < 0)
			{
				ClosePrinter(hPrinter);
				return bResult;

			}
			pDevmode = (PDEVMODE)LocalAlloc(LPTR, nSize);
			nSize = DocumentProperties(NULL, hPrinter, AUTOTEXT(pPrinterName), pDevmode, NULL, DM_OUT_BUFFER);
			if (nSize < 0)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;

			}
			//在DEVMODE中设置页面大小
			pDevmode->dmPaperSize = DMPAPER_A4;
			pDevmode->dmOrientation = DMORIENT_PORTRAIT;
			pDevmode->dmFields |= DM_PAPERSIZE | DM_ORIENTATION;
			nSize = DocumentProperties(NULL, hPrinter, AUTOTEXT(pPrinterName), pDevmode, pDevmode, DM_IN_BUFFER | DM_OUT_BUFFER);
			if (nSize < 0)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;

			}
			HDC hDC = CreateDC(NULL, AUTOTEXT(pPrinterName), NULL, pDevmode);
			if (hDC == NULL)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;
			}
			SetStretchBltMode(hDC, HALFTONE);
			int dc_page_width = ((double)GetDeviceCaps(hDC, HORZSIZE) * (double)GetDeviceCaps(hDC, LOGPIXELSX)) / 25.4;
			//宾川县的 医院是这样的 高度 要减少 20mm
			int dc_page_height = ((double)(GetDeviceCaps(hDC, VERTSIZE)) * ((double)GetDeviceCaps(hDC, LOGPIXELSY))) / 25.4;
			DOCINFO docInfo = { sizeof(DOCINFO) };
			//document name
			docInfo.lpszDocName = AUTOTEXT(Path::GetFileName(pFilePath));
			nSize = StartDoc(hDC, &docInfo);
			if (nSize <= 0)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;

			}
			nSize = StartPage(hDC);
			if (nSize <= 0)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;

			}
			CImage image;
			image.Load(AUTOTEXT(pFilePath));
			image.StretchBlt(hDC, 0, 0, dc_page_width, dc_page_height);
			EndPage(hDC);
			EndDoc(hDC);
			DeleteDC(hDC);
			LocalFree(pDevmode);
			ClosePrinter(hPrinter);
			bResult = TRUE;
		}
		else {
			/*Common::Log("打印机打开失败 " + std::string(pPrinterName));*/
		}
		return bResult;
	}
	inline bool PrinterImageA4(char* strPrinterName, char* strReportPath)
	{
		bool bResult = false;
		HANDLE     hPrinter;
		PDEVMODE  pDevmode = NULL;
		PRINTER_DEFAULTS pd = { NULL, NULL, PRINTER_ACCESS_USE };
		if (OpenPrinter(AUTOTEXT(strPrinterName), &hPrinter, &pd))
		{
			//获取默认打印机DEVMODE
			LONG nSize = DocumentProperties(NULL, hPrinter, AUTOTEXT(strPrinterName), NULL, NULL, 0);
			if (nSize < 0)
			{
				ClosePrinter(hPrinter);
				return bResult;
			}
			pDevmode = (PDEVMODE)LocalAlloc(LPTR, nSize);
			nSize = DocumentProperties(NULL, hPrinter, AUTOTEXT(strPrinterName), pDevmode, NULL, DM_OUT_BUFFER);
			if (nSize < 0)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;

			}
			//在DEVMODE中设置页面大小
			pDevmode->dmPaperSize = DMPAPER_A4;
			pDevmode->dmOrientation = DMORIENT_PORTRAIT;
			pDevmode->dmFields |= DM_PAPERSIZE | DM_ORIENTATION;
			nSize = DocumentProperties(NULL, hPrinter, AUTOTEXT(strPrinterName), pDevmode, pDevmode, DM_IN_BUFFER | DM_OUT_BUFFER);
			if (nSize < 0)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;

			}
			HDC hDC = CreateDC(NULL, AUTOTEXT(strPrinterName), NULL, pDevmode);
			if (hDC == NULL)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;
			}
			SetStretchBltMode(hDC, HALFTONE);
			int dc_page_width = GetDeviceCaps(hDC, HORZSIZE) / 25.4 * GetDeviceCaps(hDC, LOGPIXELSX);
			int dc_page_height = GetDeviceCaps(hDC, VERTSIZE) / 25.4 * GetDeviceCaps(hDC, LOGPIXELSY);
			DOCINFO docInfo = { sizeof(DOCINFO) };
			//document name
			docInfo.lpszDocName = AUTOTEXT(Path::GetFileName(strReportPath));
			nSize = StartDoc(hDC, &docInfo);
			if (nSize <= 0)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;

			}
			nSize = StartPage(hDC);
			if (nSize <= 0)
			{
				if (pDevmode)
				{
					LocalFree(pDevmode);
				}
				ClosePrinter(hPrinter);
				return bResult;

			}
			CImage image;
			image.Load(AUTOTEXT(strReportPath));
			image.StretchBlt(hDC, 0, 0, dc_page_width, dc_page_height);
			EndPage(hDC);
			EndDoc(hDC);
			DeleteDC(hDC);
			LocalFree(pDevmode);
			ClosePrinter(hPrinter);
			bResult = TRUE;
		}
		else {
			//Common::Log("打印机打开失败 "+ std::string(strPrinterName));
		}
		return bResult;
	}
	inline size_t FindPrinters(std::vector<std::string>& allPrinter)
	{
		// TODO: Add your control notification handler code here
		DWORD Flags = PRINTER_ENUM_FAVORITE | PRINTER_ENUM_LOCAL;   //local   printers
		DWORD cbBuf;
		DWORD pcReturned;
		DWORD Level = 2;
		TCHAR Name[500];
		LPPRINTER_INFO_2   pPrinterEnum = NULL;
		memset(Name, 0, sizeof(TCHAR) * 500);
		::EnumPrinters(Flags, Name, Level, NULL, 0, &cbBuf, &pcReturned);
		pPrinterEnum = (LPPRINTER_INFO_2)LocalAlloc(LPTR, cbBuf + 4);
		if (!pPrinterEnum)
		{
			return allPrinter.size();
		}
		::EnumPrinters(
			Flags, //   DWORD   Flags,   printer   object   types  
			Name, //   LPTSTR   Name,   name   of   printer   object  
			Level, //   DWORD   Level,   information   level  
			(LPBYTE)pPrinterEnum, //   LPBYTE   pPrinterEnum,   printer   information   buffer  
			cbBuf, //   DWORD   cbBuf,   size   of   printer   information   buffer
			&cbBuf, //   LPDWORD   pcbNeeded,   bytes   received   or   required  
			&pcReturned); //   LPDWORD   pcReturned   number   of   printers   enumerated  
		for (size_t i = 0; i < pcReturned; i++)
		{
			LPPRINTER_INFO_2 pInfo = &pPrinterEnum[i];

#ifdef UNICODE
			allPrinter.push_back(Text::UnicodeToANSI(pInfo->pPrinterName));
#else
			allPrinter.push_back(pInfo->pPrinterName);
#endif
		}
		LocalFree(pPrinterEnum);//释放
		return allPrinter.size();
	}
	inline bool PrinterImage(const std::string & printer, const std::string & imageFile, PageSize pageSize)
	{
		bool status = false;
		if (pageSize == PageSize::A4) {
			status = PrinterImageA4(const_cast<char*>(printer.c_str()), const_cast<char*>(imageFile.c_str()));
		}
		if (pageSize == PageSize::K16) {
			status = PrinterIamge16K(const_cast<char*>(printer.c_str()), const_cast<char*>(imageFile.c_str()));
		}
		return status;
	}
}

