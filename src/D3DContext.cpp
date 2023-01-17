#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include <tuple>

#include "..\include\VertexShader.h"
#include "..\include\PixelShader.h"


using namespace DirectX;

VertexPosColor Vertices[8] = {
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
	{XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
};

WORD Indices[36] = {
	0,1,2,0,2,3,
	4,6,5,4,7,6,
	4,5,1,4,1,0,
	3,2,6,3,6,7,
	1,5,6,1,6,2,
	4,0,3,4,3,7,
};

D3DContext::D3DContext(HINSTANCE hInstance, HWND windowHandle, bool vSync) : hInstance(hInstance), windowHandle(windowHandle), vsync(vSync) {
	RECT clientRect;
	GetClientRect(windowHandle, &clientRect);

	clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	if (InitDirectX(hInstance, windowHandle) != 0) {
		MessageBox(nullptr, TEXT("Failed to create DirectXDevice"), TEXT("Error"), MB_OK);
	}

	if (!LoadContent(windowHandle)) {
		MessageBox(nullptr, TEXT("Failed to load content"), TEXT("Error"), 0);
	}
}

D3DContext::~D3DContext() {
	UnloadContent();
	Cleanup();
}

int D3DContext::CreateDeviceAndSwapchain() {
	assert(windowHandle != 0);

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
		D3D11_SDK_VERSION, &swapChainDesc, &d3dSwapChain, &d3dDevice, &featureLevel,
		&d3dDeviceContext);

	if (hr == E_INVALIDARG)
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
			D3D11_SDK_VERSION, &swapChainDesc, &d3dSwapChain, &d3dDevice, &featureLevel,
			&d3dDeviceContext);

	if (FAILED(hr))
		return -1;

	return 0;
}

int D3DContext::CreateRenderTargetView() {
	ID3D11Texture2D* backBuffer;
	HRASSERT(d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	HRASSERT(d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &d3dRenderTargetView));
	SafeRelease(backBuffer);

	return 0;
}

int D3DContext::CreateDepthStencilView() {
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    ZeroMemory( &depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

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

	HRASSERT(d3dDevice->CreateTexture2D( &depthStencilBufferDesc, nullptr, &d3dDepthStencilBuffer ));
	HRASSERT(d3dDevice->CreateDepthStencilView(d3dDepthStencilBuffer, nullptr, &d3dDepthStencilView));

	return 0;
}

int D3DContext::CreateDepthStencilState() {
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = false;

	HRASSERT(d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &d3dDepthStencilState));

	return 0;
}

int D3DContext::CreateRasterizerState() {
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

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
	HRASSERT(d3dDevice->CreateRasterizerState(&rasterizerDesc, &d3dRasterizerState));

	return 0;
}

int D3DContext::InitDirectX(HINSTANCE hInstance, HWND windowHandle) {
	assert(CreateDeviceAndSwapchain() >= 0);
	assert(CreateRenderTargetView() >= 0);
	assert(CreateDepthStencilView() >= 0);
	assert(CreateDepthStencilState() >= 0);
	assert(CreateRasterizerState() >= 0);

	Viewport.Width = static_cast<float>(clientWidth);
	Viewport.Height = static_cast<float>(clientHeight);
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	return 0;
}

bool D3DContext::LoadContent(HWND windowHandle) {
	assert(d3dDevice);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosColor) * _countof(Vertices);
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = Vertices;

	HRASSERT(d3dDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &d3dVertexBuffer));

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * _countof(Indices);
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	resourceData.pSysMem = Indices;

	HRASSERT(d3dDevice->CreateBuffer(&indexBufferDesc, &resourceData, &d3dIndexBuffer));

	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HRASSERT(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &d3dConstantBuffers[CB_Application]));
	HRASSERT(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &d3dConstantBuffers[CB_Frame]));
	HRASSERT(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &d3dConstantBuffers[CB_Object]));
	HRASSERT(d3dDevice->CreateVertexShader(g_vs, sizeof(g_vs), nullptr, &d3dVertexShader));
	HRASSERT(d3dDevice->CreatePixelShader(g_ps, sizeof(g_ps), nullptr, &d3dPixelShader));

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Color), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRASSERT(d3dDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), g_vs, sizeof(g_vs), &d3dInputLayout));

	RECT clientRect;
	GetClientRect(windowHandle, &clientRect);

	float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), clientWidth / clientHeight, 0.1f, 100.0f);

	d3dDeviceContext->UpdateSubresource(d3dConstantBuffers[CB_Application], 0, nullptr, &ProjectionMatrix, 0, 0);

	return true;
}

void D3DContext::Update(float deltaTime) {
	assert(d3dDevice);
	assert(d3dDeviceContext);

	XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
	XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	d3dDeviceContext->UpdateSubresource(d3dConstantBuffers[CB_Frame], 0, nullptr, &ViewMatrix, 0, 0);

	static float angle = 0.0f;
	angle += 90.f * deltaTime;

	XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);

	WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
	d3dDeviceContext->UpdateSubresource(d3dConstantBuffers[CB_Object], 0, nullptr, &WorldMatrix, 0, 0);
}

void D3DContext::Clear(const float clearColor[4], float clearDepth, uint8_t clearStencil) {
	d3dDeviceContext->ClearRenderTargetView(d3dRenderTargetView, clearColor);
	d3dDeviceContext->ClearDepthStencilView(d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

void D3DContext::Present() {
	d3dSwapChain->Present(vsync ? 1 : 0, 0);
}

void D3DContext::Render() {
	assert(d3dDevice);
	assert(d3dDeviceContext);

	Clear(Colors::CornflowerBlue, 1.0f, 0);

	const UINT vertexStride = sizeof(VertexPosColor);
	const UINT offset = 0;

	d3dDeviceContext->IASetVertexBuffers(0, 1, &d3dVertexBuffer, &vertexStride, &offset);
	d3dDeviceContext->IASetInputLayout(d3dInputLayout);
	d3dDeviceContext->IASetIndexBuffer(d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dDeviceContext->VSSetShader(d3dVertexShader, nullptr, 0);
	d3dDeviceContext->VSSetConstantBuffers(0, 3, d3dConstantBuffers);

	d3dDeviceContext->RSSetState(d3dRasterizerState);
	d3dDeviceContext->RSSetViewports(1, &Viewport);
	
	d3dDeviceContext->PSSetShader(d3dPixelShader, nullptr, 0);

	d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, d3dDepthStencilView);
	d3dDeviceContext->OMSetDepthStencilState(d3dDepthStencilState, 1);

	d3dDeviceContext->DrawIndexed(_countof(Indices), 0, 0);

	Present();
}

void D3DContext::UnloadContent() {
	SafeRelease(d3dConstantBuffers[CB_Application]);
	SafeRelease(d3dConstantBuffers[CB_Frame]);
	SafeRelease(d3dConstantBuffers[CB_Object]);
	SafeRelease(d3dIndexBuffer);
	SafeRelease(d3dVertexBuffer);
	SafeRelease(d3dInputLayout);
	SafeRelease(d3dVertexShader);
	SafeRelease(d3dPixelShader);
}

void D3DContext::Cleanup() {
	SafeRelease( d3dDepthStencilView );
	SafeRelease( d3dRenderTargetView );
	SafeRelease( d3dDepthStencilBuffer );
	SafeRelease( d3dDepthStencilState );
	SafeRelease( d3dRasterizerState );
	SafeRelease( d3dSwapChain );
	SafeRelease( d3dDeviceContext );
	SafeRelease( d3dDevice );
}
