#pragma once

#include "DirectXTemplatePCH.h"

enum GBufferID {
	DIFFUSE = 0,
	NORMAL = 1,
	ZBUFFER = 2,
	GBUFFER_COUNT = 3,
};

class D3DContext {
public:
	D3DContext(HINSTANCE hInstance, HWND windowHandle, bool vSync);
	~D3DContext();
	void Clear(const float clearColor[4], float clearDepth, uint8_t clearStencil);
	void Present();

	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* d3dDeviceContext = nullptr;

	IDXGISwapChain* d3dSwapChain = nullptr;

	ID3D11RenderTargetView* BackBufferRTV = nullptr;
	ID3D11DepthStencilView* d3dDepthStencilView = nullptr;
	ID3D11Texture2D* d3dDepthStencilBuffer = nullptr;

	ID3D11DepthStencilState* d3dDepthStencilState = nullptr;
	ID3D11RasterizerState* d3dRasterizerState = nullptr;
	D3D11_VIEWPORT Viewport = { 0 };

	float clientWidth;
	float clientHeight;
private:
	bool vsync;
	HWND windowHandle;
	HINSTANCE hInstance;

	int InitDirectX(HINSTANCE hInstance, HWND windowHandle);
	int CreateDeviceAndSwapchain();
	int CreateRenderTargetView();
	int CreateDepthStencilView();
	int CreateDepthStencilState();
	int CreateRasterizerState();
	void Cleanup();

};

