#include "PCH.h"
#include "Log.h"
#include "platform/Platform.h"

#if PLATFORM_WINDOWS
#define DEBUG_PRINT(s) do{OutputDebugStringA(s);std::cout<<(s);}while(0)
#else
#error "Platform unsupported for logging"
#endif

Log::Level Log::g_logLevel = Log::INFO;
Log::Channel Log::g_logTmp = {"TMP"};

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
	case ERR: DEBUG_PRINT("ERROR "); break;
	case FATAL: DEBUG_PRINT("FATAL "); break;
	default: DEBUG_BREAK();
	}
	snprintf(buf, BUFSIZE, "%s: %s (%d): ", chan.name, file, line);
	DEBUG_PRINT(buf);

	vsnprintf(buf, BUFSIZE, fmt, args);
	DEBUG_PRINT(buf);

	DEBUG_PRINT("\n");

	if (lvl == ERR) {
		DEBUG_BREAK();
	}

	if (lvl == FATAL) {
		DEBUG_BREAK();
		CRASH();
	}

	va_end(args);
}

void Log::logBuffer(Level lvl, Channel* chan, int line, const char* file, const u8* bufptr, size_t bufsize) {
	if (lvl < g_logLevel)
		return;

	static const int BUFSIZE = 2048;
	char buf[BUFSIZE];

	switch (lvl) {
	case INFO: DEBUG_PRINT("INFO "); break;
	case WARN: DEBUG_PRINT("WARN "); break;
	case ERR: DEBUG_PRINT("ERROR "); break;
	case FATAL: DEBUG_PRINT("FATAL "); break;
	default: ASSERT(false, "Invalid error level");
	}
	snprintf(buf, BUFSIZE, "%s: %s (%d): ", chan ? chan->name : "Log", file, line);
	DEBUG_PRINT(buf);


	char* bufNullTerminated = (char*)malloc(bufsize+1);
	bufNullTerminated[bufsize] = 0;
	memcpy(bufNullTerminated, bufptr, bufsize);
	DEBUG_PRINT(bufNullTerminated);

	if (lvl == ERR) {
		DEBUG_BREAK();
	}

	if (lvl == FATAL) {
		DEBUG_BREAK();
		CRASH();
	}
}
