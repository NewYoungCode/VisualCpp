#pragma once




namespace Text {
	//寻找字符串里面字符个数
	size_t FindCount(const std::string&str, const std::string&ch_);
	//去除前后空格
	void Trim(std::string &str);
	//包含大写的字母转小写
	std::string ToLower(const std::string&str);
	//包含小写的字母转大写
	std::string ToUpper(const std::string&str);
	//ANSIToUniCode
	std::wstring ANSIToUniCode(const std::string &wstr);
	//UnicodeToANSI
	std::string UnicodeToANSI(const std::wstring &wstr);
	//UnicodeToUTF8
	std::string UnicodeToUTF8(const std::wstring &wstr);
	//ANSIToUTF8
	std::string ANSIToUTF8(const std::string &str);
	//UTF8ToANSI
	std::string UTF8ToANSI(const std::string &str);
	//截取字符串(宽字符)
	std::wstring Substr(const std::wstring &str, size_t starIndex, size_t count = std::string::npos);
	//截取字符串(窄字符)
	std::string Substr(const std::string &str, size_t starIndex, size_t count = std::string::npos);
	//直接替换字符并且返回修改个数
	size_t Replace(std::string &str, const std::string &oldText, const std::string & newText);
	//直接替换字符并且返回修改之后的字符串
	std::string ReplaceAll(const std::string &str, const std::string & oldText, const std::string & newText);
	//字符串分割
	std::vector<std::string> Split(const std::string& str, const std::string& ch);

	//删除字符串
	bool EraseString(std::string &out_in_str, const std::string& in_oldStr);

	template<typename ...T>
	void Format(LPTSTR buf, size_t strCount, LPCTSTR formatStr, T...args);

#define AUTOTEXT(str) const_cast<LPTSTR>(Text::Auto(str).c_str())

	inline auto Auto(const std::string&str) {
#ifdef UNICODE
		return ANSIToUniCode(str);
#else
		return str;
#endif 
	}

	inline	auto Auto(const std::wstring&wstr) {
#ifdef UNICODE
		return wstr;
#else
		return UnicodeToANSI(wstr);
#endif 
	}
}

//定义................................................
namespace Text {

	inline size_t FindCount(const std::string&str, const std::string&ch_) {
		size_t pos = str.find(ch_);
		size_t count = 1;
		if (pos == std::string::npos) {
			return 0;
	}
		while ((pos = str.find(ch_, pos + ch_.size())) != std::string::npos) {
			count++;
		}
		return count;
}

	template<typename ...T>
	inline	void Format(LPTSTR buf, size_t strCount, LPCTSTR formatStr, T ...args)
	{
#ifdef UNICODE
		swprintf_s((buf), strCount, formatStr, std::forward<T>(args)...);
#else
		sprintf_s((buf), strCount, formatStr, std::forward<T>(args)...);
#endif
	}

	inline void Trim(std::string &str) {
		size_t pos = str.find(" ");
		if (pos == std::string::npos) return;
		std::string newStr = str;
		//寻找头部的空格
		while (true)
		{
			if (pos == 0) {
				newStr = newStr.substr(0 + 1);
				pos = newStr.find(" ");
				continue;
			}
			break;
		}
		//寻找尾部的空格
		pos = newStr.rfind(" ");
		while (true)
		{
			if (pos == newStr.size() - 1) {
				newStr = newStr.substr(0, pos);
				pos = newStr.rfind(" ");
				continue;
			}
			break;
		}
		str = newStr;
	}
	inline std::string ToUpper(const std::string&str) {
		char* cStr = (char*)malloc(str.size() + 1);
		size_t pos = 0;
		for (auto ch : str) {
			char newCh = ch;
			if (ch >= 97 && ch <= 122) {
				newCh = ch - 32;
			}
			cStr[pos] = newCh;
			pos++;
		}
		cStr[str.size()] = 0;
		std::string newStr = cStr;
		free(cStr);
		return newStr;
	}
	inline std::string ToLower(const std::string&str) {
		char* cStr = (char*)malloc(str.size() + 1);
		size_t pos = 0;
		for (auto ch : str) {
			char newCh = ch;
			if (ch >= 65 && ch <= 90) {
				newCh = ch + 32;
			}
			cStr[pos] = newCh;
			pos++;
		}
		cStr[str.size()] = 0;
		std::string newStr = cStr;
		free(cStr);
		return newStr;
	}
	inline size_t  Replace(std::string  &str, const std::string & oldText, const std::string & newText)
	{
		std::string &newStr = str;
		size_t pos;
		pos = str.find(oldText);
		size_t count = 0;
		for (; pos != std::string::npos;) {
			newStr.replace(pos, oldText.size(), newText);
			count++;
			pos = newStr.find(oldText);
		}
		return count;
	}
	inline std::string ReplaceAll(const std::string &str, const std::string & oldText, const std::string & newText) {
		std::string newStr = str;
		Replace(newStr, oldText, newText);
		return newStr;
	}
	inline std::vector<std::string> Split(const std::string& str, const std::string& ch_) {
		std::vector<std::string> arr;
		if (str.empty()) return arr;
		std::string buf = str;
		size_t pos = buf.find(ch_);
		if (pos == std::string::npos) {
			arr.push_back(str);
			return arr;
		}
		for (; pos != std::string::npos;) {
			arr.push_back(buf.substr(0, pos));
			buf = buf.erase(0, pos + ch_.size());
			pos = buf.find(ch_);
			if (pos == std::string::npos) {
				arr.push_back(buf);
			}
		}
		return arr;
	}
	inline std::wstring Substr(const std::wstring &str, size_t starIndex, size_t count) {
		return str.substr(starIndex, count);
	}
	inline std::string Substr(const std::string &str, size_t starIndex, size_t count) {
		std::wstring newStr = Text::ANSIToUniCode(str).substr(starIndex, count);
		return Text::UnicodeToANSI(newStr);
	}
	inline std::wstring ANSIToUniCode(const std::string &str)
	{
		int bytes = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
		std::wstring wstrCmd;
		wstrCmd.resize(bytes);
		bytes = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), const_cast<wchar_t*>(wstrCmd.c_str()), wstrCmd.size());
		return wstrCmd;
	}
	inline std::string UnicodeToANSI(const std::wstring &wstr)
	{
		int bytes = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
		std::string strCmd;
		strCmd.resize(bytes);
		bytes = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), const_cast<char*>(strCmd.data()), strCmd.size(), NULL, NULL);
		return strCmd;
	}
	inline std::string UnicodeToUTF8(const std::wstring &wstr)
	{
		int bytes = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
		std::string strCmd;
		strCmd.resize(bytes);
		bytes = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), const_cast<char*>(strCmd.data()), strCmd.size(), NULL, NULL);
		return strCmd;
	}
	inline std::string UTF8ToANSI(const std::string& str)
	{
		int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		memset(pwBuf, 0, nwLen * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);
		WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
		std::string strRet = pBuf;

		delete[]pBuf;
		delete[]pwBuf;
		pBuf = NULL;
		pwBuf = NULL;
		return strRet;
	}
	inline std::string ANSIToUTF8(const std::string& str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2);
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);
		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
		std::string strRet(pBuf);
		delete[]pwBuf;
		delete[]pBuf;
		pwBuf = NULL;
		pBuf = NULL;
		return strRet;
	}

	inline bool EraseString(std::string &out_in_str, const std::string& in_oldStr) {
		auto pos = out_in_str.find(in_oldStr);
		if (pos== 0) {
			out_in_str.erase(pos, in_oldStr.size());
			return true;
		}
		return false;
	}
};

#ifdef UNICODE
#define TOUNICODE(str) str
#define TOANSI(str) Text::UnicodeToANSI(str).c_str()
#define BASESTRING std::wstring 
#else
#define TOANSI(str) str
#define TOUNICODE(str) Text::ANSIToUniCode(str).c_str()
#define BASESTRING std::string 
#endif

#ifdef  UNICODE
//the TString is UNICODE
struct TString :public BASESTRING {
#else
//the TString is ANSI 
struct TString :public BASESTRING {
#endif
public:
	TString():BASESTRING(){}
#ifdef UNICODE
	TString(const std::string&str) :BASESTRING(Text::ANSIToUniCode(str)) {}
	TString(const std::wstring&str) :BASESTRING(str) {}

	TString(const char*str) :BASESTRING(Text::ANSIToUniCode(str)) {}
	TString(const wchar_t* str) :BASESTRING(str) {}
#else
	TString(const std::wstring&str) : BASESTRING(Text::UnicodeToANSI(str)) {}
	TString(const std::string&str) :BASESTRING(str) {}

	TString(const wchar_t*str) : BASESTRING(Text::UnicodeToANSI(str)) {}
	TString(const char* str) :BASESTRING(str) {}
#endif

#undef BASESTRING
};
