#pragma once

#include <typeindex>

#include "types/Types.h"

u64 hashString(const char* s);

template <typename T>
struct DefaultHash {
	u32 operator() (const T& x) const { return x.hash(); }
};

// lowbias32 from Hash Prospector https://github.com/skeeto/hash-prospector
inline u32 lowbias32(u32 x)
{
    x ^= x >> 16;
    x *= 0x7feb352d;
    x ^= x >> 15;
    x *= 0x846ca68b;
    x ^= x >> 16;
    return x;
}

template <>
struct DefaultHash<u32> {
	u32 operator() (const u32 x) const { return lowbias32(x); }
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

//template <typename T, typename H=DefaultHash<T>> inline u32 defaultHash(T& x) { return H()(x); }

template <typename T>
struct TypeID {
};

#define TYPEID_IMPL_STRUCT_NS(NS, T) namespace NS { struct T; } template<> struct TypeID<NS::T> { static constexpr u32 get() { return __LINE__; } };
#define TYPEID_IMPL_STRUCT(T) struct T; template<> struct TypeID<T> { static constexpr u32 get() { return __LINE__; } };

TYPEID_IMPL_STRUCT_NS(ECS, TestComponent);
TYPEID_IMPL_STRUCT_NS(ECS, AnotherComponent);
TYPEID_IMPL_STRUCT(SpriteComponent);
