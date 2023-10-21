#pragma once

#define WINDOWS 0
#define LINUX 1

#define PLATFORM WINDOWS

#if PLATFORM == WINDOWS
#define PLATFORM_WINDOWS
#define WINDOWS_ONLY
#else
#error "Platform not implemented"
#define WINDOWS_ONLY #error "This header can only be imported on Windows platforms"
#endif

namespace PAL {

class WindowHandle;

void mouseSetCapture(WindowHandle* h);
void mouseReleaseCapture(WindowHandle* h);

}
