#pragma once
namespace Base64 {
	static unsigned char alphabet_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static unsigned char reverse_map[] =
	{
		 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62, 255, 255, 255, 63,
		 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255, 255, 255, 255, 255,
		 255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255,
		 255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 255, 255, 255, 255, 255
	};
	extern std::string base64_encode(const unsigned char *text, unsigned int text_len);
	extern std::string  base64_decode(const unsigned char *code, unsigned int code_len);

	inline std::string base64_encode(const unsigned char *text, unsigned int text_len)
	{
		unsigned int i, j;
		std::string base64Str;
		for (i = 0, j = 0; i + 3 <= text_len; i += 3)
		{
			base64Str += alphabet_map[text[i] >> 2];                             //ȡ����һ���ַ���ǰ6λ���ҳ���Ӧ�Ľ���ַ�
			base64Str += alphabet_map[((text[i] << 4) & 0x30) | (text[i + 1] >> 4)];     //����һ���ַ��ĺ�2λ��ڶ����ַ���ǰ4λ������ϲ��ҵ���Ӧ�Ľ���ַ�
			base64Str += alphabet_map[((text[i + 1] << 2) & 0x3c) | (text[i + 2] >> 6)];   //���ڶ����ַ��ĺ�4λ��������ַ���ǰ2λ��ϲ��ҳ���Ӧ�Ľ���ַ�
			base64Str += alphabet_map[text[i + 2] & 0x3f];                         //ȡ���������ַ��ĺ�6λ���ҳ�����ַ�
		}
		if (i < text_len)
		{
			unsigned int tail = text_len - i;
			if (tail == 1)
			{
				base64Str += alphabet_map[text[i] >> 2];
				base64Str += alphabet_map[(text[i] << 4) & 0x30];
				base64Str += '=';
				base64Str += '=';
			}
			else //tail==2
			{
				base64Str += alphabet_map[text[i] >> 2];
				base64Str += alphabet_map[((text[i] << 4) & 0x30) | (text[i + 1] >> 4)];
				base64Str += alphabet_map[(text[i + 1] << 2) & 0x3c];
				base64Str += '=';
			}
		}
		return base64Str;
	}
	inline std::string  base64_decode(const unsigned char *code, unsigned int code_len)
	{
		//assert();  //��������������ش�������ֹ����ִ�С�4�ı�����
		if ((code_len & 0x03) == 0) {
			return "";
		}
		std::string str;

		unsigned int i, j = 0;
		unsigned char quad[4];
		for (i = 0; i < code_len; i += 4)
		{
			for (unsigned int k = 0; k < 4; k++)
			{
				quad[k] = reverse_map[code[i + k]];//���飬ÿ���ĸ��ֱ�����ת��Ϊbase64���ڵ�ʮ������
			}

			//assert(quad[0] < 64 && quad[1] < 64);
			if (quad[0] < 64 && quad[1] < 64) {
				return "";
			}

			str += (quad[0] << 2) | (quad[1] >> 4); //ȡ����һ���ַ���Ӧbase64���ʮ��������ǰ6λ��ڶ����ַ���Ӧbase64���ʮ��������ǰ2λ�������

			if (quad[2] >= 64)
				break;
			else if (quad[3] >= 64)
			{
				str += (quad[1] << 4) | (quad[2] >> 2); //ȡ���ڶ����ַ���Ӧbase64���ʮ�������ĺ�4λ��������ַ���Ӧbase64���ʮ��������ǰ4λ�������
				break;
			}
			else
			{
				str += (quad[1] << 4) | (quad[2] >> 2);
				str += (quad[2] << 6) | quad[3];//ȡ���������ַ���Ӧbase64���ʮ�������ĺ�2λ���4���ַ��������
			}
		}
		return str;
	}
};