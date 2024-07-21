#pragma once

#include <typeindex>

#include "types/Types.h"

u64 hashString(const char* s);

template <typename T>
struct DefaultHash {
	u32 operator() (T& x) const { return x.HASH_FUNCTION_NOT_IMPLEMENTED_FOR_TYPE_T(); }
};

template <>
struct DefaultHash<u32> {
	u32 operator() (const u32 x) const { return x; }
};

struct CStringHash {
	u32 operator() (const char* s) const { return (u32)hashString(s); }
};

template <>
struct DefaultHash<const char*> : public CStringHash {};

template <>
struct DefaultHash<std::type_index> {
	u32 operator() (const std::type_index x) const { return x.hash_code(); }
};
