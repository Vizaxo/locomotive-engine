#pragma once
#include "types/Types.h"

namespace Log {

enum Level {
	INFO,
	WARN,
	ERR,
	FATAL,
};

extern Level g_logLevel;

struct Channel {
	const char* name;
};

extern Channel g_logTmp;

#define LOG(lvl, chan, fmt, ...) logStr(lvl, chan, __LINE__, __FILE__, fmt,  __VA_ARGS__)
#define LOG_BUFFER(lvl, chan, bufptr, bufsize) logBuffer(lvl, chan, __LINE__, __FILE__, bufptr, bufsize)

void logStr(Level lvl, Channel& chan, int line, const char* file, const char* fmt, ...);
void logBuffer(Level lvl, Channel* chan, int line, const char* file, const u8* bufptr, size_t bufsize);

}
