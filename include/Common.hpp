//#pragma once
//#include <malloc.h>
//#include <stdio.h>
//#include <string>
//#include <Windows.h>
//#include <windowsx.h>
//#include <fstream>
//#include <io.h>
//#include <tchar.h>
//#include <vector>
//#include <sstream>
//#include <time.h>
//#include <mutex>
//#include <Text.hpp>
//#include <FileSystem.hpp>
//#pragma warning(disable:4996)
////操作ini文件的类
//class ConfigIni {
//private:
//	size_t buffSize = 129;
//	std::string filename;
//	DWORD GetValue(const std::string &section, const std::string &key, const std::string &defaultValue, const std::string &filename, std::string&outResult)const {
//		char* buff = new char[buffSize];//数据量
//		memset(buff, 0, buffSize);
//		long char_count = GetPrivateProfileStringA(section.c_str(), key.c_str(), defaultValue.c_str(), buff, buffSize - 1, filename.c_str());
//		outResult = buff;
//		delete buff;
//		return char_count;
//	}
//	bool SetValue(const std::string & section, const std::string & key, const std::string & Value, const std::string & absoluteFilename)const {
//		return WritePrivateProfileStringA(section.c_str(), key.c_str(), Value.c_str(), absoluteFilename.c_str()) == 0 ? false : true;
//	}
//public:
//	void LoadFromFile(const std::string& filename, size_t buffSize = 128) {
//		this->buffSize = buffSize + 1;
//		this->filename = filename;
//		if (!File::Exists(filename)) {
//			printf("file not Found ! Created !\n");
//			std::ofstream ofs(filename, std::ios::app);
//			ofs.close();
//		}
//	}
//	//FileName //一定要绝对路径
//	ConfigIni(const std::string &filename, size_t buffSize = 128) {
//		LoadFromFile(filename, buffSize);
//	}
//	ConfigIni() {}
//	//读取ini中的字符
//	std::string ReadString(const std::string &key, const std::string& defaultValue = "", const std::string &section = "setting") const {
//		std::string outResult;
//		GetValue(section, key, defaultValue, filename, outResult);
//		return  outResult;
//	}
//	//读取ini中的数字
//	float ReadFloat(const std::string &key, float defaultValue = 0, const std::string &section = "setting") const {
//		try
//		{
//			std::string outResult;
//			GetValue(section, key, std::to_string(defaultValue), filename, outResult);
//			return std::stof(outResult);
//		}
//		catch (const std::exception& ex)
//		{
//			printf("%s\n", ex.what());
//			return defaultValue;
//		}
//	}
//	//读取ini中的int数字
//	int ReadInt(const std::string &key, int defaultValue = 0, const std::string &section = "setting") const {
//		try
//		{
//			std::string outResult;
//			GetValue(section, key, std::to_string(defaultValue), filename, outResult);
//			return std::stoi(outResult);
//		}
//		catch (const std::exception& ex)
//		{
//			printf("%s\n", ex.what());
//			return defaultValue;
//		}
//	}
//
//	//写入ini
//	bool WriteValue(const std::string &key, const std::string &value, const std::string &section = "setting")const {
//		return SetValue(section, key, value, filename);
//	}
//};
////感觉c#的委托很好用 所以抄袭一个
//template<class out, class...in>
//class Func {
//	std::vector<std::function<out(in...)>> callbacks;
//public:
//	void operator+=(const std::function<out(in...)>& callback) {
//		callbacks.push_back(callback);
//	}
//	void operator-=(size_t eventid) {
//		//...
//	}
//	void operator()(in... args) {
//		for (auto&it : callbacks) {
//			it(std::forward<in>(args)...);
//		}
//	}
//};
//
//template<class T>
////统计函数耗时时间
//time_t StopWatch(const T&_func) {
//	auto beg_t = std::chrono::system_clock::now();    //起始时间
//	_func();
//	auto end_t = std::chrono::system_clock::now();    //结束时间
//	std::chrono::duration<double> diff = end_t - beg_t;//时间间隔s
//	time_t total = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();//时间间隔ms
//	printf("Elapsed :%d ms\n", total);
//	return total;
//}
//
////线程池
//class ThreadPool {
//
//};
//
////常用工具
//namespace Common {
//	//设置开机启动
//	bool SetAutoBoot(bool autoBoot, const std::string& filename = "");
//	//获取是否自动启动状态
//	bool GetAutoBootStatus(const std::string& filename = "");
//	template<typename T >
//	//申请二维数组
//	T** MallocTwoArr(size_t row, size_t col);
//	template<typename T >
//	//是否二维数组长度
//	void FreeTwoArr(T**arr, size_t row, size_t col);
//	//打印日志
//	void Log(const std::string &log);
//}
////系统时间的工具
//namespace DateTime {
//	namespace Now {
//		std::string ToString(const std::string &format = "yyyy-MM-dd hh::mm:ss");
//	}
//}
////****************************光荣的分割线*******************************
//namespace DateTime {
//	namespace Now {
//		inline  std::string ToString(const std::string &format) {
//			SYSTEMTIME time;
//			GetLocalTime(&time);
//#ifndef ushort
//#define ushort WORD
//#endif
//			std::string  year = std::to_string((ushort)time.wYear);//年
//			std::string  Month = std::to_string((ushort)time.wMonth);//月
//			std::string  Day = std::to_string((ushort)time.wDay);//日
//			std::string Hour = std::to_string((ushort)time.wHour);//时
//			std::string Minute = std::to_string((ushort)time.wMinute);//分
//			std::string Second = std::to_string((ushort)time.wSecond);//秒
//			std::string wMilliseconds = std::to_string((ushort)time.wMilliseconds);//毫秒
//			Month = Month.size() == 1U ? "0" + Month : Month;
//			Day = Day.size() == 1U ? "0" + Day : Day;
//			Hour = Hour.size() == 1U ? "0" + Hour : Hour;
//			Minute = Minute.size() == 1U ? "0" + Minute : Minute;
//			Second = Second.size() == 1U ? "0" + Second : Second;
//			std::string formatStr = format;
//			formatStr = Text::ReplaceAll(formatStr, "yyyy", year);
//			formatStr = Text::ReplaceAll(formatStr, "MM", Month);
//			formatStr = Text::ReplaceAll(formatStr, "dd", Day);
//			formatStr = Text::ReplaceAll(formatStr, "hh", Hour);
//			formatStr = Text::ReplaceAll(formatStr, "mm", Minute);
//			formatStr = Text::ReplaceAll(formatStr, "ss", Second);
//			formatStr = Text::ReplaceAll(formatStr, "mmmm", wMilliseconds);
//			return formatStr;
//		}
//	}
//}
//namespace Common {
//	template<typename T >
//	inline T** MallocTwoArr(size_t row, size_t col) {
//		T **arr = new T*[row];
//		for (size_t i = 0; i < row; i++)
//		{
//			arr[i] = new T[col];
//		}
//		return arr;
//	}
//	template<typename T >
//	inline void FreeTwoArr(T**arr, size_t row) {
//		for (size_t i = 0; i < row; i++)
//		{
//			delete arr[i];
//		}
//		delete arr;
//	}
//	inline void Log(const std::string &text) {
//		OutputDebugStringA(text.c_str());
//		OutputDebugStringA("\n");
//		std::string appFilename = Path::GetModuleFileName();
//		std::string appName = Path::GetFileNameWithoutExtension(appFilename);
//		std::string dir = Path::GetDirectoryName(appFilename) + "/" + appName + "_Log";
//		std::string logFilename = dir + "/" + DateTime::Now::ToString("yyyy-MM-dd") + ".log";
//		Path::Create(dir);
//		std::string logText = DateTime::Now::ToString("hh:mm:ss ") + text;
//		std::ofstream file(logFilename.c_str(), std::ios::app);
//		file << logText.c_str() << std::endl;
//		file.flush();
//		file.close();
//	}
//};