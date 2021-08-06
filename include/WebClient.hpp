#pragma once
#include <Common.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
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
#pragma  warning (disable:4996) //禁用4996警告
//接收响应文档
extern size_t receive_data(char *contents, size_t size, size_t nmemb, void *respone);
//接收下载文件
extern size_t receive_filename(char *contents, size_t size, size_t nmemb, void *_fielname);
class Proxy {
public:
	std::string host;
	unsigned int port;
	curl_proxytype proxytype = CURLPROXY_SOCKS5;
	std::string user;
	std::string password;
	Proxy(const std::string &host, size_t port, const curl_proxytype &curl_proxytype = CURLPROXY_SOCKS5, const std::string& user = "", const std::string&password = "") {
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
//curl_global_init(CURL_GLOBAL_ALL);初始化curl
class WebClient
{
private:
#define curl_get 0
#define curl_post 1
	CURL*  Init(const std::string &url, std::string& resp, int timeOut);
	void CleanUp(CURL* curl, CURLcode code);
public:
	std::string Cookies;
	WebClient();
	virtual ~WebClient();
	Proxy *Proxy = NULL;
	CURLcode DownloadFile(const std::string & strUrl, const std::string & filename, int nTimeout = 20);
	CURLcode HttpGet(const std::string & strUrl, std::string & strResponse, int nTimeout = 20);
	CURLcode HttpPost(const std::string & strUrl, const std::string & data, std::string & respone, int nTimeout = 20);
	CURLcode SubmitForm(const std::string &strUrl, const std::vector<Form::Field>& fieldValues, std::string& respone, int nTimeout = 20);
	CURLcode UploadFile(const std::string &strUrl, const std::string &filename, const std::string &field, std::string &respone, int nTimeout = 30);
	CURLcode FtpDownLoad(const std::string& strUrl, const std::string&user, const std::string&pwd, const std::string &outFileName, int nTimeout = 30);
};
inline WebClient::WebClient() {}
inline WebClient::~WebClient() {
	if (this->Proxy) {
		delete this->Proxy;
	}
	Cookies.clear();
};
//定义
inline size_t receive_data(char *contents, size_t size, size_t nmemb, void *respone) {
	size_t count = size * nmemb;
	std::string *str = (std::string*)respone;
	(*str).append(contents, count);
	return count;
};
inline size_t receive_filename(char *contents, size_t size, size_t nmemb, void *fielname) {
	std::ofstream  ofs((char*)fielname, std::ios::app | std::ios::binary);
	size_t count = size * nmemb;
	ofs.write(contents, count);
	ofs.flush();
	ofs.close();
	return count;
};
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
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponse);
	return curl;
};
inline void WebClient::CleanUp(CURL* curl, CURLcode code) {
	//如果执行成功,
	if (code == CURLE_OK)
	{
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
	curl_easy_cleanup(curl);
};
inline CURLcode WebClient::HttpGet(const std::string & strUrl, std::string & strResponse, int nTimeout) {
	CURLcode res = CURLE_OK;
	CURL* curl = Init(strUrl, strResponse, nTimeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	res = curl_easy_perform(curl);
	CleanUp(curl, res);
	return res;
};
inline CURLcode WebClient::HttpPost(const std::string &url, const std::string &data, std::string &respone, int _timeout) {
	CURLcode code = CURLE_OK;
	CURL* curl = Init(url, respone, _timeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	curl_easy_setopt(curl, CURLOPT_POST, true);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	code = curl_easy_perform(curl);
	CleanUp(curl, code);
	return code;
};
inline CURLcode WebClient::UploadFile(const std::string &url, const std::string &filename, const std::string &field, std::string &respone, int _timeout) {
	CURLcode code = CURLE_OK;
	CURL* curl = Init(url, respone, _timeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	struct curl_httppost *formpost = 0;
	struct curl_httppost *lastptr = 0;
	curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, field.c_str(), CURLFORM_FILE, filename.c_str(), CURLFORM_END);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	code = curl_easy_perform(curl);
	if (formpost) {
		curl_formfree(formpost);
	}
	CleanUp(curl, code);
	return code;
};
inline CURLcode WebClient::SubmitForm(const std::string &strUrl, const std::vector<Form::Field>& fieldValues, std::string& respone, int nTimeout) {
	CURLcode code = CURLE_OK;
	CURL* curl = Init(strUrl, respone, nTimeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	struct curl_slist*      headerlist = NULL;
	struct curl_httppost*    formpost = NULL;
	struct curl_httppost*    lastptr = NULL;
	// 设置表头，表头内容可能不同
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
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
	code = curl_easy_perform(curl);
	CleanUp(curl, code);
	if (formpost) {
		curl_formfree(formpost);
	}
	curl_slist_free_all(headerlist);
	return code;

};
inline CURLcode WebClient::DownloadFile(const std::string &url, const std::string &_filename, int nTimeout) {
	CURLcode code = CURLE_OK;
	std::string resp;
	CURL* curl = Init(url, resp, nTimeout);
	if (!curl) {
		return CURLE_FAILED_INIT;
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_filename);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, _filename.c_str());
	code = curl_easy_perform(curl);
	CleanUp(curl, code);
	return code;
};
inline CURLcode WebClient::FtpDownLoad(const std::string& strUrl, const std::string&user, const std::string&pwd, const std::string &outFileName, int nTimeout) {
	CURLcode code = CURLE_OK;
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
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_filename);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, outFileName.c_str());
	code = curl_easy_perform(curl);
	CleanUp(curl, code);
	return code;
};
