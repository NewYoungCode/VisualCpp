#pragma once
#pragma  warning (disable:4099) //����û��pdb�ļ�����
#include <json/json.h>
#ifdef  _DEBUG
#pragma comment (lib,"json_libd.lib")
#else
#pragma comment (lib,"json_lib.lib")
#endif