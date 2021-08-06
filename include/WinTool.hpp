#pragma once
#include <TlHelp32.h>
#include <ShlObj.h>
#include <ShObjIdl.h>
#include <ShlGuid.h>
#include <psapi.h>
namespace WinTool {
	typedef struct {
		unsigned long process_id;
		HWND best_handle;
	}handle_data;
	//给进程提权
	BOOL EnablePrivilege(HANDLE process = NULL);
	//创建桌面快捷方式
	bool CreateDesktopLnk(const std::string &pragmaFilename, const std::string &LnkName = "", const std::string& cmdline = "", const std::string& iconFilename = "");
	//设置程序自启动
	bool SetAutoBoot(const std::string& filename, bool enable = true);
	//获取程序自启动状态
	bool GetAutoBootStatus(const std::string& filename);
	BOOL IsMainWindow(HWND handle);
	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);
	HWND FindMainWindow(DWORD process_id);
	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);
	//获取进程信息
	std::vector<PROCESSENTRY32> FindProcessInfo(const std::string& _proccname);
	//获取进程ID集合
	std::vector<DWORD> FindProcessId(const std::string& proccname);
	//获取进程文件路径
	std::string FindProcessFilename(DWORD pid);
	//关闭所有进程
	int CloseProcess(const std::vector<DWORD>& pids);
	//使用进程ID关闭进程
	bool CloseProcess(DWORD pid);
}
namespace WinTool {
	inline BOOL IsMainWindow(HWND handle)
	{
		return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
	}
	inline HWND FindMainWindow(DWORD process_id)
	{
		handle_data data;
		data.process_id = process_id;
		data.best_handle = 0;
		EnumWindows(EnumWindowsCallback, (LPARAM)&data);
		return data.best_handle;
	}
	inline BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
	{
		handle_data& data = *(handle_data*)lParam;
		unsigned long process_id = 0;
		GetWindowThreadProcessId(handle, &process_id);
		if (data.process_id != process_id || !IsMainWindow(handle)) {
			return TRUE;
		}
		data.best_handle = handle;
		return FALSE;
	}
	inline std::vector<PROCESSENTRY32> FindProcessInfo(const std::string& _proccname) {

		std::vector<PROCESSENTRY32> infos;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		for (auto status = Process32First(hSnapshot, &pe); status != FALSE; status = Process32Next(hSnapshot, &pe)) {
			pe.dwSize = sizeof(PROCESSENTRY32);
#ifdef UNICODE
			std::string item = Text::UnicodeToANSI(pe.szExeFile);
#else
			std::string item = pe.szExeFile;
#endif
			if (item == _proccname) {
				infos.push_back(pe);
			}
			//printf("%s %d\n", item.data(),pe.th32ProcessID);
		}
		CloseHandle(hSnapshot);
		return infos;
	}
	inline std::vector<DWORD> FindProcessId(const std::string& _proccname)
	{
		std::vector<DWORD> pids;
		auto list = FindProcessInfo(_proccname);
		for (auto &it : list) {
			pids.push_back(it.th32ProcessID);
		}
		return pids;
	}
	inline std::string FindProcessFilename(DWORD pid)
	{
		char buf[MAX_PATH]{ 0 };
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		DWORD result = ::GetModuleFileNameExA(hProcess, NULL, buf, sizeof(buf) - 1);
		CloseHandle(hProcess);
		return buf;
	}
	inline int CloseProcess(const std::vector<DWORD>& pids) {
		size_t count = 0;
		for (auto item : pids) {
			count += CloseProcess(item);
		}
		return count;
	}
	inline bool CloseProcess(DWORD unProcessID)
	{
		HANDLE bExitCode = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE
			| PROCESS_ALL_ACCESS, FALSE, unProcessID);
		if (bExitCode)
		{
			BOOL bFlag = ::TerminateProcess(bExitCode, 0);
			CloseHandle(bExitCode);
			return true;
		}
		return false;
	}
	inline bool GetAutoBootStatus(const std::string& filename) {
		std::string bootstart = filename.empty() ? Path::StartFileName() : filename;
		std::string appName = Path::GetFileNameWithoutExtension(bootstart);
		bool bResult = false;
		HKEY subKey;
		REGSAM dwFlag = KEY_ALL_ACCESS;
		BOOL is64Bit = FALSE;
		::IsWow64Process(::GetCurrentProcess(), &is64Bit);
		if (is64Bit)
		{
			dwFlag |= KEY_WOW64_64KEY;
		}
		if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\", NULL, dwFlag, &subKey))
		{
			return bResult;
		}
		//3、判断注册表项是否已经存在
		char strDir[MAX_PATH] = {};
		DWORD nLength = MAX_PATH;
		LSTATUS status = RegGetValueA(subKey, NULL, appName.c_str(), REG_SZ, NULL, strDir, &nLength);
		if (status != ERROR_SUCCESS) {
			if (bootstart == strDir) {
				bResult = true;
			}
		}
		::RegCloseKey(subKey);
		return bResult;
	}
	inline bool SetAutoBoot(const std::string& filename, bool bStatus)
	{
		//如果需要设置成自启动而且已经是自启动就返回true
		if (bStatus && GetAutoBootStatus(filename)) {
			return true;
		}
		if (!bStatus && !GetAutoBootStatus(filename)) {
			return true;
		}
		std::string bootstart = filename.empty() ? Path::StartFileName() : filename;
		std::string appName = Path::GetFileNameWithoutExtension(bootstart);
		bool bResult = false;
		HKEY subKey;
		REGSAM dwFlag = KEY_ALL_ACCESS;
		BOOL is64Bit = FALSE;
		::IsWow64Process(::GetCurrentProcess(), &is64Bit);
		if (is64Bit)
		{
			dwFlag |= KEY_WOW64_64KEY;
		}
		if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\", NULL, dwFlag, &subKey))
		{
			return bResult;
		}
		if (bStatus)
		{

			if (ERROR_SUCCESS == ::RegSetValueExA(subKey, appName.c_str(), NULL, REG_SZ, (PBYTE)bootstart.c_str(), bootstart.size()))
			{
				bResult = true;
			}
		}
		else
		{
			if (ERROR_SUCCESS == ::RegDeleteValueA(subKey, appName.c_str()))
			{
				bResult = true;
			}
		}
		::RegCloseKey(subKey);
		return bResult;
	}
	inline BOOL EnablePrivilege(HANDLE process)
	{
		// 得到令牌句柄
		HANDLE hToken = NULL;
		bool bResult = false;
		if (OpenProcessToken(process ? process : ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY | TOKEN_READ, &hToken)) {
			// 得到特权值
			LUID luid;
			if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
				// 提升令牌句柄权限
				TOKEN_PRIVILEGES tp = {};
				tp.PrivilegeCount = 1;
				tp.Privileges[0].Luid = luid;
				tp.Privileges[0].Attributes = 1 ? SE_PRIVILEGE_ENABLED : 0;
				if (AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL)) {
					bResult = true;
				}
			}
		}
		// 关闭令牌句柄
		CloseHandle(hToken);
		return bResult;
	}
	inline bool CreateDesktopLnk(const std::string &pragmaFilename, const std::string &LnkName, const std::string& cmdline, const std::string& iconFilename) {
		HRESULT hr = CoInitialize(NULL);
		bool bResult = false;
		if (SUCCEEDED(hr))
		{
			IShellLink *pShellLink;
			hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
			if (SUCCEEDED(hr))
			{
				pShellLink->SetPath(AUTOTEXT(pragmaFilename));
				pShellLink->SetWorkingDirectory(AUTOTEXT(Path::GetDirectoryName(pragmaFilename)));
				pShellLink->SetArguments(AUTOTEXT(cmdline));
				if (!iconFilename.empty())
				{
					pShellLink->SetIconLocation(AUTOTEXT(iconFilename), 0);
				}
				IPersistFile* pPersistFile;
				hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
				if (SUCCEEDED(hr))
				{
					char buf[MAX_PATH]{ 0 };
					SHGetSpecialFolderPathA(0, buf, CSIDL_DESKTOPDIRECTORY, 0);//获取当前用户桌面路径
					std::string userDesktop(buf);
					if (!LnkName.empty()) {
						userDesktop += "\\" + LnkName + ".lnk";
					}
					else {
						userDesktop += "\\" + Path::GetFileNameWithoutExtension(pragmaFilename) + ".lnk";
					}
					//设置快捷方式地址
					File::Delete(userDesktop);//删除旧的
					hr = pPersistFile->Save(Text::ANSIToUniCode(userDesktop).c_str(), FALSE);
					if (SUCCEEDED(hr))
					{
						bResult = true;
					}
					pPersistFile->Release();
				}
				pShellLink->Release();
			}
		}
		CoUninitialize();
		return bResult;
	}
};