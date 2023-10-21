#include "PCH.h"

// Windows implementation of generic platform API

#include "platform/Platform.h"
#ifdef PLATFORM_WINDOWS
#include "platform/windows/Windows.h"

namespace PAL {

void mouseSetCapture(WindowHandle* h) { SetCapture(h->hwnd); }
void mouseReleaseCapture(WindowHandle* h) { ReleaseCapture(); }
void setCursorPosition(v2i p) { SetCursorPos(p.x, p.y); }
void showCursor(bool show) { ShowCursor(show); }
v2i getCursorPos() {
	POINT p;
	GetCursorPos(&p);
	return {p.x, p.y};
}

}

#endif