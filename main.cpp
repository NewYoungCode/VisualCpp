﻿#include "os.h"
#include "Base64.hpp"
#include "FileSystem.hpp"
#include "GdiPlus.hpp"
#include "Md5.hpp"
#include "Printer.hpp"
#include "Socket.hpp"
#include "Text.hpp"
#include "WinTool.hpp"
#include "ConfigIni.hpp"
#include "WebClient.hpp"
#include "Func.hpp"

#include "TString.hpp"

int main(int count, char*args[]) {

	//TString sss = "666";
	//sss.append("哈哈哈");
	//sss.append("金泰没");

	//int aaaa = 666;
	//int intSzie = sizeof(int);
	//int stringSzie = sizeof(std::string);
	//std::string *bbbb = (std::string*)(void*)&aaaa;
	//Socket *aaaaaa = new Socket(Socket::TCP);
	//size_t aaaaa;
	//std::function<void()> clickEvent = NULL;
	//Func<int, int, int> funcs;
	////绑定事件
	//auto eventid = funcs += [=](int a, int b)->int {
	//	printf("%d\n", a + b);
	//	return a + b;
	//};
	////执行绑定的事件
	//funcs(100, 200);
	////移除某个事件
	//funcs -= eventid;
	////for (;;) {
		WebClient wc;
		wc.AddHeader("User-Agent", "不知名浏览器");
		wc.AddHeader("User-Agentx", "xxx");
		wc.RemoveHeader("User-Agentxx");
		std::string str;
		auto code = wc.HttpGet("http://baidu.com", str);
	////	break;
	////}

	//AUTOTEXT("");

	////WebClient wc;
	////auto code = wc.DownloadFile("https://www.nyzhishan.com/static_2021/d/2.7.6/fotiaoqiang-2.7.6-1-Setup.exe", Path::StartPath() + "\\aa.exe", test);
	////return 0;
	////std::vector<int> lll;
	//WinTool::SetAutoBoot();
	//WinTool::SetAutoBoot(Path::StartFileName(), false);

	//for (auto &it : WinTool::FindProcessInfo("")) {
	//	if (!WinTool::Is64BitPorcess(it.th32ProcessID)) {
	//		printf("%s\n", it.szExeFile);
	//	}
	//}
	/*ConfigIni ini(Path::StartPath() + "\\test.ini", "settint");
	ini.WriteValue("name", "666");
	auto xx = ini.ReadString("name");
	auto a = WinTool::FindProcessId("WeChatApp.exe").at(0);
	auto b = WinTool::FindProcessFilename(a);
	WinTool::EnablePrivilege();
	std::vector<std::string> all;
	Printer::FindPrinters(all);
	while (true)
	{
		WinTool::SetAutoBoot(b);
		WinTool::SetAutoBoot(b, false);
	}*/
	return 0;
}