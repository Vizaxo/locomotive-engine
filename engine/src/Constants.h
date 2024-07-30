#pragma once

#define API_DIRECTX_11 1

#define RENDERDOC_CAPTURE_CALLSTACKS 0

#if _DEBUG
#define DEBUG_ENABLE_ASSERTS 1
#if RENDERDOC_CAPTURE_CALLSTACKS
// Renderdoc can't capture callstaks if program uses dbghelp.dll. Might be able to dynamically unload it before renderdoc captures.
#define DEBUG_LOG_ALLOCATIONS 0
#else
#define DEBUG_LOG_ALLOCATIONS 1
#endif
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

//#define ENGINE_SHADER(s) L"..\\locomotive-engine\\engine\\src\\shaders\\" s ".hlsl"
#define ENGINE_SHADER(s) ENGINE_DIR "\\src\\shaders\\" s ".hlsl"
