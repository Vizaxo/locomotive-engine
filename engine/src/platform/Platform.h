#pragma once

#include "types/Vector.h"

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

namespace PAL {

struct WindowHandle;

void mouseSetCapture(WindowHandle* h);
void mouseReleaseCapture(WindowHandle* h);
void setCursorPosition(v2i p);
void showCursor(bool show);
v2i getCursorPos();
void printBacktrace(int stackFramesToPrint, int skipFirstNFrames);

}
