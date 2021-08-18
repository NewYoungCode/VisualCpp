#pragma once
#include <time.h>
namespace Time {
	namespace Now {
		inline  std::string ToString(const std::string &format = "yyyy-MM-dd hh:mm:ss") {
			SYSTEMTIME time;
			GetLocalTime(&time);
			std::string  year = std::to_string(time.wYear);//年
			std::string  Month = std::to_string(time.wMonth);//月
			std::string  Day = std::to_string(time.wDay);//日
			std::string Hour = std::to_string(time.wHour);//时
			std::string Minute = std::to_string(time.wMinute);//分
			std::string Second = std::to_string(time.wSecond);//秒
			std::string wMilliseconds = std::to_string(time.wMilliseconds);//毫秒
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
		//统计函数耗时时间
		inline time_t StopWatch(const T&_func) {
			auto beg_t = std::chrono::system_clock::now();    //起始时间
			_func();
			auto end_t = std::chrono::system_clock::now();    //结束时间
			std::chrono::duration<double> diff = end_t - beg_t;//时间间隔s
			time_t total = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();//时间间隔ms
			printf("Elapsed :%d ms\n", total);
			return total;
		}
	}
};