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


//定义
inline WebClient::WebClient() {
	if (!g_curl_bInit) {
		curl_global_init(CURL_GLOBAL_ALL); //初始化curl
		g_curl_bInit = true;
	}
}
inline WebClient::~WebClient() {
	if (this->Proxy) {
		delete this->Proxy;
	}
}
inline size_t g_curl_receive_callback(char *contents, size_t size, size_t nmemb, void *respone) {
	size_t count = size * nmemb;
	std::string *str = (std::string*)respone;
	(*str).append(contents, count);
	return count;
};
inline size_t g_curl_download_callback(char *contents, size_t size, size_t nmemb, void *fielname) {
	std::ofstream  ofs((char*)fielname, std::ios::app | std::ios::binary);
	size_t count = size * nmemb;
	ofs.write(contents, count);
	ofs.flush();
	ofs.close();
	return count;
}
inline int g_curl_progress_callback(void *ptr, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	if (dltotal != 0 && ptr) {
		(*(ProgressFunc*)ptr)(dltotal, dlnow, dlnow / (dltotal * 1.0) * 100);
	}
	return 0;
}
inline CURL* WebClient::Init(const std::string &strUrl, std::string& strResponse, int nTimeout) {
	CURL* curl = curl_easy_init();
	if (!curl) {
		return curl;
	}
	if (Proxy) {
		curl_easy_setopt(curl, CURLOPT_PROXYTYPE, Proxy->proxytype); //代理模式
		curl_easy_setopt(curl, CURLOPT_PROXY, Proxy->host.c_str()); //代理服务器地址
		curl_easy_setopt(curl, CURLOPT_PROXYPORT, Proxy->port); //代理服务器端口
		if (!Proxy->user.empty() && !Proxy->password.empty()) {
			curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, std::string(Proxy->user + ":" + Proxy->password).c_str()); //用户密码
		}
	}
	//初始化cookie引擎
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");    //初始化cookie引擎,才能正确接收到cookie数据.
	if (!Cookies.empty()) {
		curl_easy_setopt(curl, CURLOPT_COOKIE, Cookies.c_str());
	}
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);	// 验证对方的SSL证书
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);	//根据主机验证证书的名称
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());//设置url地址
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);//设置超时
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, g_curl_receive_callback);//接受回调
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponse);//

	for (auto &it : Header) {
		auto hd = it.first + ":" + it.second;
		curl_header = curl_slist_append(curl_header, hd.c_str());
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_header);
	return curl;
};
inline long WebClient::CleanUp(CURL* curl, CURLcode code) {
	long RESPONSE_CODE = (int)code;
	//如果执行成功,
	if (code == CURLE_OK)
	{
		//获取响应的状态码
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &RESPONSE_CODE);

		struct curl_slist *cookies = NULL;
		curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);       //获得cookie数据  
		int i = 1;
		while (cookies)
		{
			//printf("%s\n", cookies->data);
			Cookies.append(cookies->data);
			cookies = cookies->next;
			i++;
		}
	}

	if (curl_header) {
		curl_slist_free_all(curl_header);
		curl_header = NULL;
	}
	curl_easy_cleanup(curl);
	return RESPONSE_CODE;
};
inline int WebClient::HttpGet(const std::string & strUrl, std::string & strResponse, int nTimeout) {
	CURL* curl = Init(strUrl, strResponse, nTimeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	CURLcode code = curl_easy_perform(curl);
	return CleanUp(curl, code);
};
inline int WebClient::HttpPost(const std::string &url, const std::string &data, std::string &respone, int _timeout) {

	CURL* curl = Init(url, respone, _timeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	curl_easy_setopt(curl, CURLOPT_POST, true);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	CURLcode code = curl_easy_perform(curl);
	return CleanUp(curl, code);
};
inline int WebClient::UploadFile(const std::string &url, const std::string &filename, const std::string &field, std::string &respone, const ProgressFunc&progressCallback, int _timeout) {

	CURL* curl = Init(url, respone, _timeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	struct curl_httppost *formpost = 0;
	struct curl_httppost *lastptr = 0;
	curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, field.c_str(), CURLFORM_FILE, filename.c_str(), CURLFORM_END);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);


	if (progressCallback) {
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);//接受上传下载进度
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progressCallback);//将函数回调函数设置传入指针
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, g_curl_progress_callback);//进度回调
	}

	CURLcode  code = curl_easy_perform(curl);
	if (formpost) {
		curl_formfree(formpost);
	}


	return CleanUp(curl, code);
};
inline int WebClient::SubmitForm(const std::string &strUrl, const std::vector<Form::Field>& fieldValues, std::string& respone, int nTimeout) {

	AddHeader("Content-Type", "multipart/form-data");
	CURL* curl = Init(strUrl, respone, nTimeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	struct curl_httppost*    formpost = NULL;
	struct curl_httppost*    lastptr = NULL;
	// 设置表头，表头内容可能不同
	for (auto &item : fieldValues) {
		if (item.FieldType == Form::FieldType::File) {
			curl_formadd(&formpost, &lastptr,
				CURLFORM_COPYNAME, item.FieldName.c_str(),
				CURLFORM_FILE, item.FileName.c_str(),
				CURLFORM_CONTENTTYPE, "application/octet-stream",
				CURLFORM_END);
		}
		if (item.FieldType == Form::FieldType::Text) {
			curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, item.FieldName.c_str(), CURLFORM_COPYCONTENTS, item.FieldValue.c_str(), CURLFORM_END);
		}
	}
	// 设置表单参数
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	CURLcode code = curl_easy_perform(curl);
	CleanUp(curl, code);
	if (formpost) {
		curl_formfree(formpost);
	}
	return CleanUp(curl, code);

};
inline int WebClient::DownloadFile(const std::string &url, const std::string &_filename, const ProgressFunc&progressCallback, int nTimeout) {
	std::string resp;
	CURL* curl = Init(url, resp, nTimeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	File::Delete(_filename);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, g_curl_download_callback);//接受下载的回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, _filename.c_str()); //保存文件名

	if (progressCallback) {
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);//接受上传下载进度
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progressCallback);//将函数回调函数设置传入指针
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, g_curl_progress_callback);//进度回调
	}

	CURLcode  code = curl_easy_perform(curl);
	return CleanUp(curl, code);
};
inline int WebClient::FtpDownLoad(const std::string& strUrl, const std::string&user, const std::string&pwd, const std::string &outFileName, int nTimeout) {

	std::string resp;
	CURL* curl = Init(strUrl, resp, nTimeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	if (!user.empty() && !pwd.empty()) {
		curl_easy_setopt(curl, CURLOPT_USERPWD, (user + ":" + pwd).c_str());
	}
	else {
		curl_easy_setopt(curl, CURLOPT_USERPWD, "");
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, g_curl_download_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, outFileName.c_str());
	CURLcode code = curl_easy_perform(curl);
	return CleanUp(curl, code);
}
inline void WebClient::AddHeader(const std::string & key, const std::string & value)
{
	Header.insert(std::pair<std::string, std::string>(key, value));
}
inline void WebClient::RemoveHeader(const std::string & key)
{
	Header.erase(key);
};

