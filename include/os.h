#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <fstream>
#include <vector>
#include <sstream>
#include <mutex>
#include <memory>
#include <map>
#include <io.h>
#include <Windows.h>
#include <winuser.h>
#include <assert.h>

#include "Text.hpp" //��������
#include "FileSystem.hpp"//�����ļ�
#include "Time.hpp" //����ʱ��
#include "WinTool.hpp" //win32���ú���

namespace comm {
	inline void Log(const std::string &text) {
		OutputDebugStringA(text.c_str());
		OutputDebugStringA(("\n"));
		std::string appFilename = Path::StartFileName();
		std::string appName = Path::GetFileNameWithoutExtension(appFilename);
		std::string dir = Path::GetDirectoryName(appFilename) + "/" + appName + "_Log";
		std::string logFilename = dir + "/" + Time::Now::ToString("yyyy-MM-dd") + ".log";
		Path::Create(dir);
		std::string logText = Time::Now::ToString("hh:mm:ss ") + text;
		std::ofstream file(logFilename.c_str(), std::ios::app);
		printf("%s\n", logText.c_str());
		file << logText.c_str() << std::endl;
		file.flush();
		file.close();
		//ɾ���������־ ֻ�����������־
		auto today = Time::Now::ToString("yyyy-MM-dd");
		auto files = Path::SearchFiles(dir, "*.log");
		for (auto&it : files) {
			auto t = Path::GetFileNameWithoutExtension(it);
			if (t.size() < 10 || t.substr(0, 10) != today) {
				File::Delete(it);
			}
		}
	}
}