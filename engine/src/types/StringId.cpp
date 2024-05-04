#include "PCH.h"
#include "StringId.h"

#include <vector>

#include "utils/Hashing.h"

std::vector<StringId> stringIdManager;

StringId& internStringId(const char* s) {
	return stringIdManager.emplace_back(
#if _DEBUG
		takeOwnership(s),
#endif
		hashString(s)
	);
}