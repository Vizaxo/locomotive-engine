#pragma once

namespace Log {

enum Level {
	INFO,
	WARN,
	FATAL,
};

extern Level g_logLevel;

struct Channel {
	const char* name;
};

extern Channel g_logTmp;

#define LOG(lvl, chan, fmt, ...) logStr(lvl, chan, __LINE__, __FILE__, fmt,  __VA_ARGS__)

void logStr(Level lvl, Channel& chan, int line, const char* file, const char* fmt, ...);

}
