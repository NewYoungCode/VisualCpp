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


void  test(curl_off_t total, curl_off_t now, float rate) {
	printf("%g \n",  rate);
	return;
}

int main(int count, char*args[]) {

	WebClient wc;
	wc.DownloadFile("https://www.nyzhishan.com/static_2021/d/2.7.6/fotiaoqiang-2.7.6-1-Setup.exe", Path::StartPath() + "\\aa.exe", test);
	return 0;

	ConfigIni ini(Path::StartPath() + "\\test.ini", "settint");
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
	}
	return 0;
}