#pragma once

#include "PCH.h"

#include "platform/Platform.h"
WINDOWS_ONLY

namespace PAL {

struct WindowHandle {
	HWND hwnd;
};

}
