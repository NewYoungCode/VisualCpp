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
////����ini�ļ�����
//class ConfigIni {
//private:
//	size_t buffSize = 129;
//	std::string filename;
//	DWORD GetValue(const std::string &section, const std::string &key, const std::string &defaultValue, const std::string &filename, std::string&outResult)const {
//		char* buff = new char[buffSize];//������
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
//	//FileName //һ��Ҫ����·��
//	ConfigIni(const std::string &filename, size_t buffSize = 128) {
//		LoadFromFile(filename, buffSize);
//	}
//	ConfigIni() {}
//	//��ȡini�е��ַ�
//	std::string ReadString(const std::string &key, const std::string& defaultValue = "", const std::string &section = "setting") const {
//		std::string outResult;
//		GetValue(section, key, defaultValue, filename, outResult);
//		return  outResult;
//	}
//	//��ȡini�е�����
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
//	//��ȡini�е�int����
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
//	//д��ini
//	bool WriteValue(const std::string &key, const std::string &value, const std::string &section = "setting")const {
//		return SetValue(section, key, value, filename);
//	}
//};
////�о�c#��ί�кܺ��� ���Գ�Ϯһ��
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
////ͳ�ƺ�����ʱʱ��
//time_t StopWatch(const T&_func) {
//	auto beg_t = std::chrono::system_clock::now();    //��ʼʱ��
//	_func();
//	auto end_t = std::chrono::system_clock::now();    //����ʱ��
//	std::chrono::duration<double> diff = end_t - beg_t;//ʱ����s
//	time_t total = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();//ʱ����ms
//	printf("Elapsed :%d ms\n", total);
//	return total;
//}
//
////�̳߳�
//class ThreadPool {
//
//};
//
////���ù���
//namespace Common {
//	//���ÿ�������
//	bool SetAutoBoot(bool autoBoot, const std::string& filename = "");
//	//��ȡ�Ƿ��Զ�����״̬
//	bool GetAutoBootStatus(const std::string& filename = "");
//	template<typename T >
//	//�����ά����
//	T** MallocTwoArr(size_t row, size_t col);
//	template<typename T >
//	//�Ƿ��ά���鳤��
//	void FreeTwoArr(T**arr, size_t row, size_t col);
//	//��ӡ��־
//	void Log(const std::string &log);
//}
////ϵͳʱ��Ĺ���
//namespace DateTime {
//	namespace Now {
//		std::string ToString(const std::string &format = "yyyy-MM-dd hh::mm:ss");
//	}
//}
////****************************���ٵķָ���*******************************
//namespace DateTime {
//	namespace Now {
//		inline  std::string ToString(const std::string &format) {
//			SYSTEMTIME time;
//			GetLocalTime(&time);
//#ifndef ushort
//#define ushort WORD
//#endif
//			std::string  year = std::to_string((ushort)time.wYear);//��
//			std::string  Month = std::to_string((ushort)time.wMonth);//��
//			std::string  Day = std::to_string((ushort)time.wDay);//��
//			std::string Hour = std::to_string((ushort)time.wHour);//ʱ
//			std::string Minute = std::to_string((ushort)time.wMinute);//��
//			std::string Second = std::to_string((ushort)time.wSecond);//��
//			std::string wMilliseconds = std::to_string((ushort)time.wMilliseconds);//����
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