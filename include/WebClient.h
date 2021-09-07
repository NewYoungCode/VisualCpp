#pragma once
#ifndef CURL_STATICLIB
#define CURL_STATICLIB
#endif // !CURL_STATICLIB
#include "curl/curl.h"
#include "curl/easy.h"
#ifdef _DEBUG
#pragma comment(lib,"libcurld.lib")
#else
#pragma comment(lib,"libcurl.lib")
#endif // !_DEBUG
#pragma comment(lib,"Crypt32.lib")//curl需要的库
#pragma comment(lib,"wldap32.lib")//curl需要的库
#pragma comment(lib,"ws2_32.lib") //curl需要的库
//接收响应body
extern size_t g_curl_receive_callback(char *contents, size_t size, size_t nmemb, void *respone);
//接收下载文件
extern size_t g_curl_download_callback(char *contents, size_t size, size_t nmemb, void *_fielname);
//接受上传或者下载进度
extern int g_curl_progress_callback(void *ptr, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
//下载进度回调函数模型
typedef std::function<void(curl_off_t total, curl_off_t now, float rate)> ProgressFunc;
//curl的初始化
static bool g_curl_bInit = false;

class Proxy {
public:
	std::string host;
	unsigned int port;
	curl_proxytype proxytype = CURLPROXY_SOCKS5;
	std::string user;
	std::string password;
	inline Proxy(const std::string &host, size_t port, const curl_proxytype &curl_proxytype = CURLPROXY_SOCKS5, const std::string& user = "", const std::string&password = "") {
		this->host = host;
		this->port = port;
		this->proxytype = curl_proxytype;
		this->user = user;
		this->password = password;
	}
};
namespace Form {
	//字段类型
	enum FieldType :char
	{
		None,
		Text,
		File
	};
	class Field {
	public:
		FieldType FieldType = FieldType::None;
		std::string FieldName;
		std::string FieldValue;

		std::string FileName;
		Field(const std::string & FieldName, const std::string &ValueOrFullFileName, Form::FieldType FieldType = Form::FieldType::Text) {
			this->FieldName = FieldName;
			this->FieldType = FieldType;
			if (FieldType == Form::FieldType::File) {
				this->FieldValue = Path::GetFileName(ValueOrFullFileName);
				this->FileName = ValueOrFullFileName;
			}
			else {
				this->FieldValue = ValueOrFullFileName;
			}
		}
	};
}

class WebClient
{
private:
	CURL*  Init(const std::string &url, std::string& resp, int timeOut);
	long CleanUp(CURL* curl, CURLcode code);
	std::map<std::string, std::string> Header;
	struct curl_slist *curl_header = NULL;
public:
	std::string Cookies;
	WebClient();
	virtual ~WebClient();
	Proxy *Proxy = NULL;
	void AddHeader(const std::string&key, const std::string&value);
	void RemoveHeader(const std::string&key);
	int DownloadFile(const std::string & strUrl, const std::string & filename, const ProgressFunc&progressCallback = NULL, int nTimeout = 20);
	int HttpGet(const std::string & strUrl, std::string & strResponse, int nTimeout = 20);
	int HttpPost(const std::string & strUrl, const std::string & data, std::string & respone, int nTimeout = 20);
	int SubmitForm(const std::string &strUrl, const std::vector<Form::Field>& fieldValues, std::string& respone, int nTimeout = 20);
	int UploadFile(const std::string &strUrl, const std::string &filename, const std::string &field, std::string &respone, const ProgressFunc&progressCallback = NULL, int nTimeout = 30);
	int FtpDownLoad(const std::string& strUrl, const std::string&user, const std::string&pwd, const std::string &outFileName, int nTimeout = 30);
};
