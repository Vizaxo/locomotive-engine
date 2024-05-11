#pragma once

#include <windows.h>
#include <windowsx.h>
#include <atlbase.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdint>
#include <fstream>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <string>
#include <thread>
#include <tuple>
#include <tuple>
#include <variant>
#include <vector>
#include <vector>
#include <vector>

#include "stb/stb_image.h"

//TODO: fix these include paths
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib") //TODO: specialise depending on configuration
#pragma comment(lib, "winmm.lib")

#include "Constants.h"
#include "Utils.h"