#include "DirectXTemplatePCH.h"

#include "D3DContext.h"

#include "..\include\VertexShader.h"
#include "..\include\PixelShader.h"


using namespace DirectX;

ID3D11Device* g_d3dDevice = nullptr;
ID3D11DeviceContext* g_d3dDeviceContext = nullptr;
IDXGISwapChain* g_d3dSwapChain = nullptr;

ID3D11RenderTargetView* g_d3dRenderTargetView = nullptr;
ID3D11DepthStencilView* g_d3dDepthStencilView = nullptr;
ID3D11Texture2D* g_d3dDepthStencilBuffer = nullptr;

ID3D11DepthStencilState* g_d3dDepthStencilState = nullptr;
ID3D11RasterizerState* g_d3dRasterizerState = nullptr;
D3D11_VIEWPORT g_Viewport = { 0 };

ID3D11InputLayout* g_d3dInputLayout = nullptr;
ID3D11Buffer* g_d3dVertexBuffer = nullptr;
ID3D11Buffer* g_d3dIndexBuffer = nullptr;

ID3D11VertexShader* g_d3dVertexShader = nullptr;
ID3D11PixelShader* g_d3dPixelShader = nullptr;

bool vsync;

int InitDirectX(HINSTANCE hInstance, HWND windowHandle);
bool LoadContent(HWND windowHandle);
void UnloadContent();
void Cleanup();

enum ConstantBuffer {
	CB_Application,
	CB_Frame,
	CB_Object,
	NumConstantBuffers
};

ID3D11Buffer* g_d3dConstantBuffers[NumConstantBuffers];

DirectX::XMMATRIX g_WorldMatrix;
DirectX::XMMATRIX g_ViewMatrix;
DirectX::XMMATRIX g_ProjectionMatrix;

typedef struct VertexPosColor {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
} VertexPosColor;

VertexPosColor g_Vertices[8] = {
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
	{XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
};

WORD g_Indices[36] = {
	0,1,2,0,2,3,
	4,6,5,4,7,6,
	4,5,1,4,1,0,
	3,2,6,3,6,7,
	1,5,6,1,6,2,
	4,0,3,4,3,7,
};

D3DContext::D3DContext(HINSTANCE hInstance, HWND windowHandle, bool vSync) {
	if (InitDirectX(hInstance, windowHandle) != 0) {
		MessageBox(nullptr, TEXT("Failed to create DirectXDevice"), TEXT("Error"), MB_OK);
	}

	if (!LoadContent(windowHandle)) {
		MessageBox(nullptr, TEXT("Failed to load content"), TEXT("Error"), 0);
	}

	vsync = vSync;
}

D3DContext::~D3DContext() {
	UnloadContent();
	Cleanup();
}

int InitDirectX(HINSTANCE hInstance, HWND windowHandle) {
	assert(windowHandle != 0);

	RECT clientRect;
	GetClientRect(windowHandle, &clientRect);

	float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate = { 0, 1 }; // QueryRefreshRate(clientWidth, clientHeight, vSync);
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

	UINT createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &g_d3dSwapChain, &g_d3dDevice, &featureLevel,
		&g_d3dDeviceContext);

	if (hr == E_INVALIDARG)
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels)-1,
			D3D11_SDK_VERSION, &swapChainDesc, &g_d3dSwapChain, &g_d3dDevice, &featureLevel,
			&g_d3dDeviceContext);

	if (FAILED(hr))
		return -1;

	ID3D11Texture2D* backBuffer;
	hr = g_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
		return -1;

	hr = g_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &g_d3dRenderTargetView);
	if (FAILED(hr))
		return -1;

	SafeRelease(backBuffer);

    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    ZeroMemory( &depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC) );

    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.Width = clientWidth;
    depthStencilBufferDesc.Height = clientHeight;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.SampleDesc.Count = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    hr = g_d3dDevice->CreateTexture2D( &depthStencilBufferDesc, nullptr, &g_d3dDepthStencilBuffer );
	if (FAILED(hr))
		return -1;

	hr = g_d3dDevice->CreateDepthStencilView(g_d3dDepthStencilBuffer, nullptr, &g_d3dDepthStencilView);
	if (FAILED(hr))
		return -1;

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = false;

	hr = g_d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &g_d3dDepthStencilState);
	if (FAILED(hr))
		return -1;

    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory( &rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC) );

    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterizer state object.
    hr = g_d3dDevice->CreateRasterizerState( &rasterizerDesc, &g_d3dRasterizerState );
	if (FAILED(hr))
		return -1;

	g_Viewport.Width = static_cast<float>(clientWidth);
	g_Viewport.Height = static_cast<float>(clientHeight);
	g_Viewport.TopLeftX = 0.0f;
	g_Viewport.TopLeftY = 0.0f;
	g_Viewport.MinDepth = 0.0f;
	g_Viewport.MaxDepth = 1.0f;

	return 0;
}

bool LoadContent(HWND windowHandle) {
	assert(g_d3dDevice);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosColor) * _countof(g_Vertices);
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = g_Vertices;

	HRESULT hr = g_d3dDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &g_d3dVertexBuffer);
	if (FAILED(hr))
		return false;


	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * _countof(g_Indices);
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	resourceData.pSysMem = g_Indices;

	hr = g_d3dDevice->CreateBuffer(&indexBufferDesc, &resourceData, &g_d3dIndexBuffer);
	if (FAILED(hr))
		return false;


	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = g_d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Application]);
	if (FAILED(hr))
		return false;

	hr = g_d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Frame]);
	if (FAILED(hr))
		return false;

	hr = g_d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Object]);
	if (FAILED(hr))
		return false;

	hr = g_d3dDevice->CreateVertexShader(g_vs, sizeof(g_vs), nullptr, &g_d3dVertexShader);
	if (FAILED(hr))
		return false;

	hr = g_d3dDevice->CreatePixelShader(g_ps, sizeof(g_ps), nullptr, &g_d3dPixelShader);
	if (FAILED(hr))
		return false;

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Color), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = g_d3dDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), g_vs, sizeof(g_vs), &g_d3dInputLayout);
	if (FAILED(hr))
		return false;

	RECT clientRect;
	GetClientRect(windowHandle, &clientRect);

	float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	g_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), clientWidth / clientHeight, 0.1f, 100.0f);

	g_d3dDeviceContext->UpdateSubresource(g_d3dConstantBuffers[CB_Application], 0, nullptr, &g_ProjectionMatrix, 0, 0);

	return true;
}

void D3DContext::Update(float deltaTime) {
	assert(g_d3dDevice);
	assert(g_d3dDeviceContext);

	XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
	XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	g_ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	g_d3dDeviceContext->UpdateSubresource(g_d3dConstantBuffers[CB_Frame], 0, nullptr, &g_ViewMatrix, 0, 0);

	static float angle = 0.0f;
	angle += 90.f * deltaTime;

	XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);

	g_WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
	g_d3dDeviceContext->UpdateSubresource(g_d3dConstantBuffers[CB_Object], 0, nullptr, &g_WorldMatrix, 0, 0);
}

void D3DContext::Clear(const float clearColor[4], float clearDepth, uint8_t clearStencil) {
	g_d3dDeviceContext->ClearRenderTargetView(g_d3dRenderTargetView, clearColor);
	g_d3dDeviceContext->ClearDepthStencilView(g_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

void D3DContext::Present() {
	g_d3dSwapChain->Present(vsync ? 1 : 0, 0);
}

void D3DContext::Render() {
	assert(g_d3dDevice);
	assert(g_d3dDeviceContext);

	Clear(Colors::CornflowerBlue, 1.0f, 0);

	const UINT vertexStride = sizeof(VertexPosColor);
	const UINT offset = 0;

	g_d3dDeviceContext->IASetVertexBuffers(0, 1, &g_d3dVertexBuffer, &vertexStride, &offset);
	g_d3dDeviceContext->IASetInputLayout(g_d3dInputLayout);
	g_d3dDeviceContext->IASetIndexBuffer(g_d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_d3dDeviceContext->VSSetShader(g_d3dVertexShader, nullptr, 0);
	g_d3dDeviceContext->VSSetConstantBuffers(0, 3, g_d3dConstantBuffers);

	g_d3dDeviceContext->RSSetState(g_d3dRasterizerState);
	g_d3dDeviceContext->RSSetViewports(1, &g_Viewport);
	
	g_d3dDeviceContext->PSSetShader(g_d3dPixelShader, nullptr, 0);

	g_d3dDeviceContext->OMSetRenderTargets(1, &g_d3dRenderTargetView, g_d3dDepthStencilView);
	g_d3dDeviceContext->OMSetDepthStencilState(g_d3dDepthStencilState, 1);

	g_d3dDeviceContext->DrawIndexed(_countof(g_Indices), 0, 0);

	Present();
}

void UnloadContent() {
	SafeRelease(g_d3dConstantBuffers[CB_Application]);
	SafeRelease(g_d3dConstantBuffers[CB_Frame]);
	SafeRelease(g_d3dConstantBuffers[CB_Object]);
	SafeRelease(g_d3dIndexBuffer);
	SafeRelease(g_d3dVertexBuffer);
	SafeRelease(g_d3dInputLayout);
	SafeRelease(g_d3dVertexShader);
	SafeRelease(g_d3dPixelShader);
}

void Cleanup() {
	SafeRelease( g_d3dDepthStencilView );
	SafeRelease( g_d3dRenderTargetView );
	SafeRelease( g_d3dDepthStencilBuffer );
	SafeRelease( g_d3dDepthStencilState );
	SafeRelease( g_d3dRasterizerState );
	SafeRelease( g_d3dSwapChain );
	SafeRelease( g_d3dDeviceContext );
	SafeRelease( g_d3dDevice );
}
