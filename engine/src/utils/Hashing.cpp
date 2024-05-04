#include "PCH.h"
#include "Hashing.h"

u64 hashString(const char* s) {
	// Polynomial rolling
	u64 p = 131;
	u64 m = 1e9 + 9;
	u64 hash = 0;
	u64 p_acc = 1;

	char c;
	for (int i = 0; (c = s[i]) != '\0'; i++) {
		hash += c * p_acc;
		p_acc *= p;
	}

	return hash % m;
}
