#pragma once

#include <string>
#include "types/Types.h"
#include "types/Pointers.h"

struct StringId {
#if _DEBUG
	RefPtr<const char> s;
#endif
	u64 id;
#if _DEBUG
	StringId(RefPtr<const char> s, u64 id) :s(s), id(id) {}
#else
	StringId(u64 id) :id(id) {}
	StringId(RefPtr<const char> s, u64 id) :id(id) {}
#endif
	//StringId(const StringId& other) :s(other.s), id(other.id) {}

	bool operator==(const StringId& other) const { return this->id == other.id; }
};

StringId internStringId(const char* s);
StringId sID(const char* s);

namespace std {
	template <> struct hash<StringId> {
		size_t operator()(const StringId& sid) const {
			return sid.id;
		}
	};
}
