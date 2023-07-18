#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Scene.h"
#include "Object.h"
#include "ExampleScene.h"
#include "Renderer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

using namespace DirectX;

const LONG g_WindowWidth = 1280;
const LONG g_WindowHeight = 720;
const LPCSTR g_WindowClassName = ("DirectXWindowClass");
const LPCSTR g_WindowName = ("D3D11 Renderer");
HWND g_WindowHandle = 0;
const bool g_EnableVSync = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);
int Run(D3DContext* d3dContext, Renderer& renderer, Scene& scene);


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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

int InitImgui(D3DContext* d3dContext, HWND hwnd) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(d3dContext->d3dDevice, d3dContext->d3dDeviceContext);

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

	if (InitImgui(&d3dContext, g_WindowHandle) != 0) {
	
		MessageBox(nullptr, TEXT("Failed to initialise Imgui"), TEXT("Error"), MB_OK);
		return -1;
	}

	Renderer renderer = {};
	renderer.Initialise(&d3dContext);
	Scene scene = buildExampleScene(&d3dContext);

	int ret = Run(&d3dContext, renderer, scene);

	cleanupExampleScene();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	return ret;
}


int Run(D3DContext* d3dContext, Renderer& renderer, Scene& scene) {
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

			renderer.RenderScene(d3dContext, scene, deltaTime);
		}
	}

	return static_cast<int>(msg.wParam);
}
