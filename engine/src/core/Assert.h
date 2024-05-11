#pragma once

// Assertions
// Errors

#include "Constants.h"
#include "Log.h"

#if DEBUG_ENABLE_ASSERTS
#define ASSERT(val, fmt, ...) do {if (!(val)) {\
		Log::logStr(Log::WARN, g_assertChannel, __LINE__, __FILE__, "Assertion failed: %s", TOSTRING(val));\
		Log::logStr(Log::WARN, g_assertChannel, __LINE__, __FILE__, fmt, __VA_ARGS__);\
		DEBUG_BREAK();\
	}}while(0);

extern Log::Channel g_assertChannel;

#else
#define ASSERT(val, fmt, ...)
#endif
