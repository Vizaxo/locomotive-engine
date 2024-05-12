#pragma once

#include "types/Box.h"
#include "types/Pointers.h"
#include "types/Vector.h"
#include "core/Log.h"

namespace PAL {

struct WindowHandle;

void mouseSetCapture(WindowHandle* h);
void mouseReleaseCapture(WindowHandle* h);
void setCursorPosition(v2i p);
void showCursor(bool show);
v2i getCursorPos();
void printBacktrace(int stackFramesToPrint, int skipFirstNFrames, Log::Channel& chan);
void initialiseConsole();
Box2i getClientRect(RefPtr<WindowHandle> h);

}

