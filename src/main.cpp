#include "DirectXTemplatePCH.h"

#include "D3DContext.h"


using namespace DirectX;

const LONG g_WindowWidth = 1280;
const LONG g_WindowHeight = 720;
const LPCSTR g_WindowClassName = ("DirectXWindowClass");
const LPCSTR g_WindowName = ("D3D11 Renderer");
HWND g_WindowHandle = 0;
const bool g_EnableVSync = true;


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);
int Run(D3DContext* d3dContext);

int InitApplication(HINSTANCE hInstance, int cmdShow) {
	WNDCLASSEXA wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = g_WindowClassName;

	if (!RegisterClassExA(&wndClass))
		return -1;

	RECT windowRect = { 0, 0, g_WindowWidth, g_WindowHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	g_WindowHandle = CreateWindowA(g_WindowClassName, g_WindowName,
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

	switch (message) {
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) {
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	if (!XMVerifyCPUSupport()) {
		MessageBox(nullptr, TEXT("Failed to verifiy DirectX support"), TEXT("Error"), MB_OK);
		return -1;
	}

	if (InitApplication(hInstance, cmdShow) != 0) {
		MessageBox(nullptr, TEXT("Failed to create application"), TEXT("Error"), MB_OK);
		return -1;
	}

	D3DContext d3dContext = D3DContext(hInstance, g_WindowHandle, g_EnableVSync);

	return Run(&d3dContext);
}

int Run(D3DContext* d3dContext) {
	MSG msg = { 0 };

	static DWORD previousTime = timeGetTime();
	static const float targetFramerate = 30.0f;
	static const float maxTimeStep = 1.0f / targetFramerate;

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			DWORD currentTime = timeGetTime();
			float deltaTime = (currentTime - previousTime) / 1000.0f;
			previousTime = currentTime;

			deltaTime = std::min<float>(deltaTime, maxTimeStep);

			d3dContext->Update(deltaTime);
			d3dContext->Render();
		}
	}

	return static_cast<int>(msg.wParam);
}
