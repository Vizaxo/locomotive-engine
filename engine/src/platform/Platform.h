#pragma once

#include "platform/Windows.h"

#define WINDOWS 0
#define LINUX 1

#define PLATFORM WINDOWS

#if PLATFORM == WINDOWS
#define PLATFORM_WINDOWS
#define PLATFORM_DATA WindowsPlatform
#else
#error "Platform not implemented"
#endif
