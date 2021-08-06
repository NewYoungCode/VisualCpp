
#include "os.h"
#include "Base64.hpp"
#include "FileSystem.hpp"
#include "GdiPlus.hpp"
#include "Md5.hpp"
#include "Printer.hpp"
#include "Socket.hpp"
#include "Text.hpp"
#include "WinTool.hpp"

std::string test(const char*str) {
	return std::string(str);
}

int main(int count, char*args[]) {
	auto a = WinTool::FindProcessId("WeChatApp.exe").at(0);
	auto b = WinTool::FindProcessFilename(a);
	WinTool::EnablePrivilege();

	std::vector<std::string> all;
	Printer::FindPrinters(all);

	while (true)
	{
		WinTool::SetAutoBoot(b);
		WinTool::SetAutoBoot(b, false);
	}

	return 0;
}