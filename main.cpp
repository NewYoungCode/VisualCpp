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

//void  test(curl_off_t total, curl_off_t now, float rate) {
//	printf("%g \n", rate);
//	return;
//}

int main(int count, char*args[]) {



	Socket *aaaaaa=new Socket(Socket::TCP);

	size_t aaaaa;

	Func<int, int, int> funcs;
	//绑定事件
	auto eventid= funcs += [=](int a,int b)->int {
		printf("%d\n",a+b);
		return a + b;
	};
	//执行绑定的事件
	funcs(100, 200);
	//移除某个事件
	funcs -= eventid;

	//for (;;) {
	//	WebClient wc;
	//	wc.AddHeader("User-Agent", "不知名浏览器");
	//	wc.AddHeader("User-Agentx", "xxx");
	//	wc.RemoveHeader("User-Agentxx");
	//	std::string str;
	//	auto code = wc.HttpGet("http://127.0.0.1/test", str);
	//	break;
	//}

	AUTOTEXT("");

	//WebClient wc;
	//auto code = wc.DownloadFile("https://www.nyzhishan.com/static_2021/d/2.7.6/fotiaoqiang-2.7.6-1-Setup.exe", Path::StartPath() + "\\aa.exe", test);
	//return 0;


	//std::vector<int> lll;
	//lll.

	WinTool::SetAutoBoot();
	WinTool::SetAutoBoot(Path::StartFileName(),false);

	for (auto &it : WinTool::FindProcessInfo("")) {
		if (!WinTool::Is64BitPorcess(it.th32ProcessID)) {
			printf("%s\n",it.szExeFile);
		}
	}
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