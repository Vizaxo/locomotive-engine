#pragma once

#include "PCH.h"

#define ARRAY_AS_BYTE_VECTOR(v) (std::vector((uint8_t*)v, (uint8_t*)v + sizeof(v)))
//#define VECTOR_TO_BYTE_VECTOR(v) (std::vector((uint8_t*)v.data(), (uint8_t*)v.data() + v.size()*))

template <typename T>
std::vector<uint8_t> toByteVector(std::vector<T> v) {
	std::vector<uint8_t> out = std::vector((uint8_t*)v.data(), (uint8_t*)v.data() + v.size() * sizeof(T));
	return std::move(out);
}

#define DEBUG_PRINT OutputDebugStringA
