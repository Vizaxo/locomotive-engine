#include "PCH.h"

// Windows implementation of generic platform API

#include "platform/Platform.h"
#ifdef PLATFORM_WINDOWS
#include "platform/windows/Windows.h"

namespace PAL {

void mouseSetCapture(WindowHandle* h) { SetCapture(h->hwnd); }
void mouseReleaseCapture(WindowHandle* h) { ReleaseCapture(); }

}

#endif