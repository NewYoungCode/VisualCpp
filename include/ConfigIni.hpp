#pragma once
class ConfigIni {
private:
	size_t buffSize = MAX_PATH;
	std::string filename;
	std::string section;
	DWORD GetValue(const std::string &section, const std::string &key, const std::string &defaultValue, const std::string &filename, std::string&outResult)const {
		std::string _section = section;
		if (section.empty()) {
			_section = this->section;
		}
		char* buff = new char[buffSize];//������
		memset(buff, 0, buffSize);
		long char_count = ::GetPrivateProfileStringA(_section.c_str(), key.c_str(), defaultValue.c_str(), buff, buffSize - 1, filename.c_str());
		outResult = buff;
		delete buff;
		return char_count;
	}
	bool SetValue(const std::string & section, const std::string & key, const std::string & Value, const std::string & absoluteFilename)const {
		
		std::string _section = section;
		if (section.empty()) {
			_section = this->section;
		}
		return ::WritePrivateProfileStringA(_section.c_str(), key.c_str(), Value.c_str(), absoluteFilename.c_str()) == 0 ? false : true;
	}
public:
	//FileName //һ��Ҫ����·��
	ConfigIni(const std::string& filename, const std::string &defaultSection, bool create = true, size_t buffSize = MAX_PATH) {
		this->buffSize = buffSize;
		this->filename = filename;
		this->section = defaultSection;
		if (!File::Exists(filename) && create) {
			printf("file not Found ! Created !\n");
			std::ofstream ofs(filename, std::ios::app);
			ofs.close();
		}
	}

	void SetDefaultSection(const std::string section) {
		this->section = section;
	}
	
	//��ȡini�е��ַ�
	std::string ReadString(const std::string &key, const std::string& defaultValue = "", const std::string &section = "") const {
		std::string outResult;
		GetValue(section, key, defaultValue, filename, outResult);
		return  outResult;
	}
	//��ȡini�е�����
	float ReadFloat(const std::string &key, float defaultValue = 0, const std::string &section = "") const {
		try
		{
			std::string outResult;
			GetValue(section, key, std::to_string(defaultValue), filename, outResult);
			return std::stof(outResult);
		}
		catch (const std::exception& ex)
		{
			printf("%s\n", ex.what());
			return defaultValue;
		}
	}
	//��ȡini�е�int����
	int ReadInt(const std::string &key, int defaultValue = 0, const std::string &section = "") const {
		try
		{
			std::string outResult;
			GetValue(section, key, std::to_string(defaultValue), filename, outResult);
			return std::stoi(outResult);
		}
		catch (const std::exception& ex)
		{
			printf("%s\n", ex.what());
			return defaultValue;
		}
	}

	//д��ini
	bool WriteValue(const std::string &key, const std::string &value, const std::string &section = "")const {
		return SetValue(section, key, value, filename);
	}
};
