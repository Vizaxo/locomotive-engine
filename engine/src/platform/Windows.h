#pragma once

#include "PCH.h"

struct WindowsPlatform  {
	HINSTANCE hInstance;
	HWND hwnd;
	bool vSync;
};
