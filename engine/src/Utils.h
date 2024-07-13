#pragma once

#define ARRAY_AS_BYTE_VECTOR(v) (std::vector((uint8_t*)v, (uint8_t*)v + sizeof(v)))
//#define VECTOR_TO_BYTE_VECTOR(v) (std::vector((uint8_t*)v.data(), (uint8_t*)v.data() + v.size()*))

#define COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

template <typename T>
std::vector<uint8_t> toByteVector(std::vector<T> v) {
	std::vector<uint8_t> out = std::vector((uint8_t*)v.data(), (uint8_t*)v.data() + v.size() * sizeof(T));
	return std::move(out);
}

#define TOSTRING2(x) #x
#define TOSTRING(x) TOSTRING2(x)


//Safely release a COM object
template<typename T>
inline void SafeRelease(T& ptr) {
	if (ptr != nullptr) {
		ptr->Release();
		ptr = nullptr;
	}
}

#define HRASSERT(x) do {\
	HRESULT hr = x;\
	assert(!FAILED(hr));\
	} while(false)
