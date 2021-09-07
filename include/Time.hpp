#pragma once
#pragma warning(disable:4996)
#include <time.h>
namespace Time {
	inline std::string ToString(time_t time_) {
		char timeStr[64]{ 0 };
		struct tm *ttime;
		ttime = ::localtime(&time_);
		strftime(timeStr, 64, "%Y-%m-%d %H:%M:%S", ttime);
		return std::string(timeStr);
	}
	inline time_t FileTimeToTime(const FILETIME &ft) {
		time_t t;
		LONGLONG  ll;
		ULARGE_INTEGER            ui;
		ui.LowPart = ft.dwLowDateTime;
		ui.HighPart = ft.dwHighDateTime;
		ll = ft.dwHighDateTime << 32 + ft.dwLowDateTime;  //��һ���ǲ��Ƕ����
		t = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
		//��ui.QuadPart����������ULONGLONGǿ��ת��Ϊtime_t��LONGLONG��������
		return t;
	}

	namespace Now {
		inline  std::string ToString(const std::string &format = "yyyy-MM-dd hh:mm:ss") {
			SYSTEMTIME time;
			GetLocalTime(&time);
			std::string  year = std::to_string(time.wYear);//��
			std::string  Month = std::to_string(time.wMonth);//��
			std::string  Day = std::to_string(time.wDay);//��
			std::string Hour = std::to_string(time.wHour);//ʱ
			std::string Minute = std::to_string(time.wMinute);//��
			std::string Second = std::to_string(time.wSecond);//��
			std::string wMilliseconds = std::to_string(time.wMilliseconds);//����
			Month = Month.size() == 1U ? "0" + Month : Month;
			Day = Day.size() == 1U ? "0" + Day : Day;
			Hour = Hour.size() == 1U ? "0" + Hour : Hour;
			Minute = Minute.size() == 1U ? "0" + Minute : Minute;
			Second = Second.size() == 1U ? "0" + Second : Second;
			std::string formatStr = format;
			formatStr = Text::ReplaceAll(formatStr, "yyyy", year);
			formatStr = Text::ReplaceAll(formatStr, "MM", Month);
			formatStr = Text::ReplaceAll(formatStr, "dd", Day);
			formatStr = Text::ReplaceAll(formatStr, "hh", Hour);
			formatStr = Text::ReplaceAll(formatStr, "mm", Minute);
			formatStr = Text::ReplaceAll(formatStr, "ss", Second);
			formatStr = Text::ReplaceAll(formatStr, "mmmm", wMilliseconds);
			return formatStr;
		}

		template<class T>
		//ͳ�ƺ�����ʱʱ��
		inline time_t StopWatch(const T&_func) {
			auto beg_t = std::chrono::system_clock::now();    //��ʼʱ��
			_func();
			auto end_t = std::chrono::system_clock::now();    //����ʱ��
			std::chrono::duration<double> diff = end_t - beg_t;//ʱ����s
			time_t total = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();//ʱ����ms
			printf("Elapsed :%d ms\n", total);
			return total;
		}
	}
};