#pragma once
#include <memory>

template<typename T>
struct Array {
};

struct StrItem {
	StrItem *Up = NULL;
	char *ptr = NULL;
	size_t memCount = 0;
	StrItem *Next = NULL;
	StrItem(const char*cstr) {
		memCount = strlen(cstr);
		ptr = new char[memCount];
		memcpy(ptr, cstr, memCount);
	}
	virtual ~StrItem() {
		if (ptr)
		{
			::free(ptr);
		}
	}
};

class TString {
private:
	StrItem *frist = NULL;
public:
	void append(const char*cstr) {
		StrItem *it = new StrItem(cstr);
		if (frist == NULL) {
			frist = it;
		}
		else {
			StrItem *temp;
			StrItem *last=NULL;
			while (temp = frist->Next)
			{
			}
			temp = it;
		}
	}
	TString(const char*cstr) {
		append(cstr);
	}

};