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
