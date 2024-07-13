#include "PCH.h"

#include "platform/Platform.h"
#ifdef PLATFORM_WINDOWS
#include "platform/Windows/Windows.h"

#include "debug/ttd.h"
#include "rhi/RHI.h"
#include "renderer/Scene.h"
#include "renderer/Renderer.h"
#include "renderer/ImGuiUtils.h"
#include "EngineMain.h"
#include "WinInput.h"
#include "Application.h"
#include "events/EventQueue.h"
#include "types/Vector.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace PAL {
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

	g_WindowHandle = CreateWindowW(g_WindowClassName, application->windowTitle,
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
		EventQueue::keyDownEvent(getKey(wParam));
		break;
	case WM_KEYUP:
		EventQueue::keyUpEvent(getKey(wParam));
		break;
	case WM_LBUTTONUP:
		EventQueue::mouseButtonUpEvent(Mouse::Button::M1);
		goto mouseevent;
	case WM_LBUTTONDOWN:
		EventQueue::mouseButtonDownEvent(Mouse::Button::M1);
		goto mouseevent;
	case WM_RBUTTONUP:
		EventQueue::mouseButtonUpEvent(Mouse::Button::M2);
		goto mouseevent;
	case WM_RBUTTONDOWN:
		EventQueue::mouseButtonDownEvent(Mouse::Button::M2);
		goto mouseevent;
	case WM_MBUTTONUP:
		EventQueue::mouseButtonUpEvent(Mouse::Button::M3);
		goto mouseevent;
	case WM_MBUTTONDOWN:
		EventQueue::mouseButtonDownEvent(Mouse::Button::M3);
		goto mouseevent;
	case WM_XBUTTONUP:
		EventQueue::mouseButtonUpEvent(Mouse::Button::M4);
		goto mouseevent;
	case WM_XBUTTONDOWN:
		EventQueue::mouseButtonDownEvent(Mouse::Button::M4);
		goto mouseevent;
	case WM_NCXBUTTONUP:
		EventQueue::mouseButtonUpEvent(Mouse::Button::M5);
		goto mouseevent;
	case WM_NCXBUTTONDOWN:
		EventQueue::mouseButtonDownEvent(Mouse::Button::M5);
		goto mouseevent;
	case WM_MOUSEMOVE:
	mouseevent:
	{
		//Do we need to do this too?
		//Mouse::setMouseButtonState(getMouseButton(wParam));

		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		EventQueue::mouseMoveEvent({x, y});
		break;
	}
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

	if (PAL::Windows::InitWindow(hInstance, cmdShow) != 0) {
		MessageBox(nullptr, TEXT("Failed to create window"), TEXT("Error"), MB_OK);
		return -1;
	}

	int ret;
	PAL::WindowHandle h = {PAL::Windows::g_WindowHandle};
	if (!(ret = Engine::init(&h, true))) {
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

#endif
