#include "PCH.h"
#include "Log.h"
#include "platform/Platform.h"

#if PLATFORM_WINDOWS
#define DEBUG_PRINT OutputDebugStringA
#else
#error "Platform unsupported for logging"
#endif

Log::Level Log::g_logLevel = Log::INFO;

void Log::logStr(Level lvl, Channel& chan, int line, const char* file, const char* fmt, ...) {
	if (lvl < g_logLevel)
		return;

	static const int BUFSIZE = 2048;
	char buf[BUFSIZE];

	va_list args;
	va_start(args, fmt);
	switch (lvl) {
	case INFO: DEBUG_PRINT("INFO "); break;
	case WARN: DEBUG_PRINT("WARN "); break;
	case FATAL: DEBUG_PRINT("FATAL "); break;
	default: return; //TODO: error
	}
	snprintf(buf, BUFSIZE, "%s: %s (%d): ", chan, file, line);
	DEBUG_PRINT(buf);

	vsnprintf(buf, BUFSIZE, fmt, args);
	DEBUG_PRINT(buf);

	DEBUG_PRINT("\n");

	if (lvl == FATAL) {
		DEBUG_BREAK();
		CRASH();
	}

	va_end(args);
}
