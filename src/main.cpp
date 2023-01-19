#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Scene.h"
#include "Object.h"


using namespace DirectX;

const LONG g_WindowWidth = 1280;
const LONG g_WindowHeight = 720;
const LPCSTR g_WindowClassName = ("DirectXWindowClass");
const LPCSTR g_WindowName = ("D3D11 Renderer");
HWND g_WindowHandle = 0;
const bool g_EnableVSync = true;

VertexPosColor cubeVertices[8] = {
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
	{XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
};

WORD cubeIndices[36] = {
	0,1,2,0,2,3,
	4,6,5,4,7,6,
	4,5,1,4,1,0,
	3,2,6,3,6,7,
	1,5,6,1,6,2,
	4,0,3,4,3,7,
};

VertexPosColor triangleVertices[3] = {
	{XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
};

WORD triangleIndices[6] = { 0,1,2,2,1,0 };


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);
int Run(D3DContext* d3dContext, Scene* scene);

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

	Scene scene = Scene();
	scene.objects.push_back(Object(&d3dContext, XMMatrixIdentity(), (UINT)8, cubeVertices, (UINT)36, cubeIndices, d3dContext.d3dPixelShader, d3dContext.d3dVertexShader));
	scene.objects.push_back(Object(&d3dContext, XMMatrixTranslation(4, 0, 2), (UINT)8, cubeVertices, (UINT)36, cubeIndices, d3dContext.d3dPixelShader, d3dContext.d3dVertexShader));
	scene.objects.push_back(Object(&d3dContext, XMMatrixTranslation(-2, 1, 2), (UINT)8, cubeVertices, (UINT)36, cubeIndices, d3dContext.d3dPixelShader, d3dContext.d3dVertexShader));
	scene.objects.push_back(Object(&d3dContext, XMMatrixTranslation(-4, 1, -2), (UINT)8, cubeVertices, (UINT)36, cubeIndices, d3dContext.d3dPixelShader, d3dContext.d3dVertexShader));
	scene.objects.push_back(Object(&d3dContext, XMMatrixTranslation(-2, -3, 3), (UINT)8, cubeVertices, (UINT)36, cubeIndices, d3dContext.d3dPixelShader, d3dContext.d3dVertexShader));
	scene.objects.push_back(Object(&d3dContext, XMMatrixTranslation(0, 1, 3), (UINT)8, cubeVertices, (UINT)36, cubeIndices, d3dContext.d3dPixelShader, d3dContext.d3dVertexShader));
	scene.objects.push_back(Object(&d3dContext, XMMatrixTranslation(2, -2, 1), _countof(triangleVertices), triangleVertices, _countof(triangleIndices), triangleIndices, d3dContext.d3dPixelShader, d3dContext.d3dVertexShader));

	return Run(&d3dContext, &scene);
}


int Run(D3DContext* d3dContext, Scene* scene) {
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
			scene->RenderScene(d3dContext, deltaTime);
		}
	}

	return static_cast<int>(msg.wParam);
}
