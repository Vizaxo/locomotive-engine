#pragma once

#include "DirectXTemplatePCH.h"

struct VertexPosColor {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

enum ConstantBuffer {
	CB_Application,
	CB_Frame,
	CB_Object,
	NumConstantBuffers
};


class D3DContext {
public:
	D3DContext(HINSTANCE hInstance, HWND windowHandle, bool vSync);
	~D3DContext();
	void Update(float deltaTime);
	void Clear(const float clearColor[4], float clearDepth, uint8_t clearStencil);
	void Present();
	void Render(float deltaTime);


	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* d3dDeviceContext = nullptr;

	IDXGISwapChain* d3dSwapChain = nullptr;

	ID3D11RenderTargetView* d3dRenderTargetView = nullptr;
	ID3D11DepthStencilView* d3dDepthStencilView = nullptr;
	ID3D11Texture2D* d3dDepthStencilBuffer = nullptr;

	ID3D11DepthStencilState* d3dDepthStencilState = nullptr;
	ID3D11RasterizerState* d3dRasterizerState = nullptr;
	D3D11_VIEWPORT Viewport = { 0 };

	ID3D11InputLayout* d3dInputLayout = nullptr;
	ID3D11Buffer* d3dVertexBuffer = nullptr;
	ID3D11Buffer* d3dIndexBuffer = nullptr;

	ID3D11VertexShader* d3dVertexShader = nullptr;
	ID3D11PixelShader* d3dPixelShader = nullptr;

	ID3D11Buffer* d3dConstantBuffers[NumConstantBuffers];

private:
	bool vsync;
	HWND windowHandle;
	HINSTANCE hInstance;
	float clientWidth;
	float clientHeight;

	int InitDirectX(HINSTANCE hInstance, HWND windowHandle);
	int CreateDeviceAndSwapchain();
	int CreateRenderTargetView();
	int CreateDepthStencilView();
	int CreateDepthStencilState();
	int CreateRasterizerState();
	bool LoadContent(HWND windowHandle);
	void UnloadContent();
	void Cleanup();

	DirectX::XMMATRIX WorldMatrix;
	DirectX::XMMATRIX ViewMatrix;
	DirectX::XMMATRIX ProjectionMatrix;

};

