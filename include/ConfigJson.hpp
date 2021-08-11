#pragma once
#include <sstream>
#include <fstream>
#include "json.hpp"
class  ConfigJson
{
private:
	Json::Value  jv;
	std::string filename;
public:
	ConfigJson() {};
	ConfigJson  operator[](const Json::Value& jv) {
		return this->jv.toStyledString();
	}
	ConfigJson(const std::string& jsonStr);
	bool LoadFromFile(const std::string& filename);
	int ReadInt(const std::string& filed,int defaultValue=0);
	std::string ReadString(const std::string& filed,const std::string &deFalut="");
	float ReadFloat(const std::string& filed,float defaultValue=0.0f);
	bool ReadBool(const std::string& filed,bool defaultValue=false);
	ConfigJson ReadJsonObj(const std::string& filed);
	void WriteValue(const std::string & filed, const Json::Value&value);
	void SaveAs(const std::string&out_filename, bool overrided = true);
	bool Save();
};

inline int ConfigJson::ReadInt(const std::string & filed, int defaultValue)
{
	try
	{
		if (jv[filed].isInt()) {
			return jv[filed].asInt();
		}
		return	std::stoi(jv[filed].asString());
	}
	catch (const std::exception& ex)
	{
		printf("%s\n", ex.what());
		return 0;
	}
}
inline std::string ConfigJson::ReadString(const std::string & filed,const std::string& dv)
{
	try
	{
		return jv[filed].asString();
	}
	catch (const std::exception& ex)
	{
		printf("%s\n", ex.what());
		return dv;
	}
}
inline float ConfigJson::ReadFloat(const std::string & filed,float dv)
{
	try
	{
		if (jv[filed].isDouble()) {
			return jv[filed].asDouble();
		}
		return	std::stof(jv[filed].asString());
	}
	catch (const std::exception& ex)
	{
		printf("%s\n", ex.what());
		return dv;
	}
}
inline bool ConfigJson::ReadBool(const std::string & filed,bool dv)
{
	try
	{
		if (jv[filed].isBool()) {
			return jv[filed].asBool();
		}
		if ((jv[filed].isInt() && jv[filed].asInt() == 1) || jv[filed].asString() == "true") {
			return true;
		}
	}
	catch (const std::exception& ex)
	{
		printf("%s\n", ex.what());

	}
	return dv;
}

inline ConfigJson ConfigJson::ReadJsonObj(const std::string& filed) {
	try
	{
		std::string jsonStr = this->jv[filed].toStyledString();
		return jsonStr;
	}
	catch (const std::exception& ex)
	{
		printf("%s\n", ex.what());
		return ConfigJson("");
	}
}


inline void ConfigJson::WriteValue(const std::string & filed, const Json::Value & value)
{
	try
	{
		jv[filed] = value;
	}
	catch (const std::exception& ex)
	{
		printf("%s\n", ex.what());
	}
}

inline bool ConfigJson::Save() {
	if (filename.empty() || !Path::Exists(filename)) {
		printf("%s\n", "Invalid save path");
	}
	bool canSave = true;
	if (Path::Exists(filename)) {
		canSave = File::Delete(filename);
	}
	if (canSave) {
		std::ofstream ofs(filename.c_str());
		Json::Reader rd;
		ofs << jv.toStyledString();
		ofs.flush();
		ofs.close();
		return true;
	}
	return canSave;
}

inline void ConfigJson::SaveAs(const std::string & filename, bool overrided)
{
	if (overrided && Path::Exists(filename)) {
		File::Delete(filename);
	}
	std::ofstream ofs(filename.c_str());
	Json::Reader rd;
	ofs << jv.toStyledString();
	ofs.flush();
	ofs.close();
}
inline ConfigJson::ConfigJson(const std::string & jsonStr)
{
	Json::Reader rd;
	rd.parse(jsonStr, jv);
}

inline bool  ConfigJson::LoadFromFile(const std::string& filename) {
	this->filename = filename;
	if (Path::Exists(filename)) {
		std::ifstream ifs(filename.c_str());
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();
		Json::Reader rd;
		if (rd.parse(ss.str(), jv)) {

			return true;
		}
	}
	return false;
}