#pragma once

#include "DirectXTemplatePCH.h"

struct WindowsPlatform  {
	HINSTANCE hInstance;
	HWND hwnd;
	bool vSync;
};
