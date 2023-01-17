#pragma once

#include <windows.h>
#include <atlbase.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <iostream>
#include <string>
#include <cstdint>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

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
	if (FAILED(hr)) {\
		return -1;\
	}\
	} while(false)