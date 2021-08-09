#include "os.h"
#include "Base64.hpp"
#include "FileSystem.hpp"
#include "GdiPlus.hpp"
#include "Md5.hpp"
#include "Printer.hpp"
#include "Socket.hpp"
#include "Text.hpp"
#include "WinTool.hpp"


class aa {
public:
	
	void print() {
		std::cout << value;
		hell0();
	}
	void hell0() {
	
	}
	aa(int v) :value(v) {

	}
private:
	aa() {
		value = 10;
	}
	int value;
};


int main(int count, char*args[]) {

	aa a;
	aa c(10);

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