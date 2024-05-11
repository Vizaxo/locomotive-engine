#pragma once

#define API_DIRECTX_11 1
#define API_DIRECTX_12 2
#define API_OPENGL 3
#define API_VULKAN 4
#define API_METAL 5

#define RENDER_API API_DIRECTX_11


#if _DEBUG
#define DEBUG_ENABLE_ASSERTS 1
#define DEBUG_LOG_ALLOCATIONS 1
#endif


#define WINDOWS 0
#define LINUX 1

#define PLATFORM WINDOWS

#if PLATFORM == WINDOWS
#define PLATFORM_WINDOWS 1
#define WINDOWS_ONLY
#else
#error "Platform not implemented"
#define WINDOWS_ONLY #error "This header can only be imported on Windows platforms"
#endif


#if PLATFORM_WINDOWS
#define DEBUG_BREAK DebugBreak
#else
#error "Define these macros for this platform"
#endif

#define CRASH() do{int* crash_abcdef = nullptr; *crash_abcdef=1;}while(0);
