#include "platform/Windows.h"

#include "PCH.h"

#include "debug/ttd.h"
#include "rhi/RHI.h"
#include "renderer/Scene.h"
#include "renderer/Object.h"
#include "renderer/Renderer.h"
#include "renderer/ImGuiUtils.h"
#include "EngineMain.h"
#include "WinInput.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Platform {
namespace Windows {

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);

const LONG g_WindowWidth = 1280;
const LONG g_WindowHeight = 720;
const LPCWSTR g_WindowClassName = (L"DirectXWindowClass");
const LPCWSTR g_WindowName = (L"D3D11 Renderer");
HWND g_WindowHandle = 0;
const bool g_EnableVSync = true;

int InitWindow(HINSTANCE hInstance, int cmdShow) {
	WNDCLASSEXW wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = g_WindowClassName;

	if (!RegisterClassExW(&wndClass))
		return -1;

	RECT windowRect = { 0, 0, g_WindowWidth, g_WindowHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	g_WindowHandle = CreateWindowW(g_WindowClassName, g_WindowName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);

	if (!g_WindowHandle) {
		int err = GetLastError();
		return -1;
	}

	ShowWindow(g_WindowHandle, cmdShow);
	UpdateWindow(g_WindowHandle);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message
	, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT paintStruct;
	HDC hDC;

    if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
        return true;

	switch (message) {
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		Engine::keyDown(getKey(wParam));
		break;
	case WM_KEYUP:
		Engine::keyUp(getKey(wParam));
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

}
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int cmdShow) {
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	if (Platform::Windows::InitWindow(hInstance, cmdShow) != 0) {
		MessageBox(nullptr, TEXT("Failed to create window"), TEXT("Error"), MB_OK);
		return -1;
	}

	int ret;
	if (!(ret = Engine::init({hInstance, Platform::Windows::g_WindowHandle, true}))) {
		return ret;
	}

	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			Engine::tick();
		}
	}

	Engine::cleanup();

	return static_cast<int>(msg.wParam);
}
