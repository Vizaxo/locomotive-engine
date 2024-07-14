#include "PCH.h"
#include "StringId.h"

#include <vector>

#include "utils/Hashing.h"

std::vector<StringId> stringIdManager;

StringId internStringId(const char* s) {
	// TODO: this is continuously growing when fetching resources
	return stringIdManager.emplace_back(
#if _DEBUG
		s,
#endif
		hashString(s)
	);
}

StringId sID(const char* s) {
	return internStringId(s);
}