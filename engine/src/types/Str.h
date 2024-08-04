#pragma once

struct str {
	const char* s = nullptr;
	bool needsFree = false;
	str() {}
	str(const char* inS) : s(inS), needsFree(false) {}
	str(char* inS) : s(inS), needsFree(true) {}
	str(const std::string& inS) {
		size_t size = sizeof(char) * (inS.length() + 1);
		s = (char*)malloc(size);
		strcpy_s((char*)s, size, inS.c_str());
		needsFree = true;
	}

	str& operator=(const str& other) {
		if (needsFree) {
			free((char*)s);
		}
		
		if (other.needsFree) {
			size_t size = strlen(other.s) + 1;
			s = (char*)malloc(size);
			strcpy_s((char*)s, size, other.s);
			needsFree = true;
		} else {
			s = other.s;
			needsFree = false;
		}

		return *this;
	}

	str(str&& other) {
		s = other.s;
		needsFree = other.needsFree;
		other.s = nullptr;
		other.needsFree = false;
	}
	~str() {
		if (needsFree) {
			free((char*)s);
		}
	}
};

