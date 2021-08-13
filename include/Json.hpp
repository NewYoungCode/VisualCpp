#pragma once
#pragma  warning (disable:4099) //禁用没有pdb文件警告
#include <json/json.h>
#ifdef  _DEBUG
#pragma comment (lib,"json_libd.lib")
#else
#pragma comment (lib,"json_lib.lib")
#endif
namespace Json {
	inline bool parse(const std::string&jsonStr, Json::Value&outValue) {
		Json::Reader rd;
		return rd.parse(jsonStr, outValue);
	}
};
