#include "D3DContext.h"
#include <tuple>


using namespace DirectX;

D3DContext::D3DContext(HINSTANCE hInstance, HWND windowHandle, bool vSync) : hInstance(hInstance), windowHandle(windowHandle), vsync(vSync) {
	RECT clientRect;
	GetClientRect(windowHandle, &clientRect);

	clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	if (InitDirectX(hInstance, windowHandle) != 0) {
		MessageBox(nullptr, TEXT("Failed to create DirectXDevice"), TEXT("Error"), MB_OK);
	}
}

D3DContext::~D3DContext() {
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
	RECT clientRect;
	GetClientRect(windowHandle, &clientRect);

	clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	ID3D11Texture2D* backBuffer;
	HRASSERT(d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	HRASSERT(d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &BackBufferRTV));
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
#define check(f) do {if (f != 0) return -1;} while(0);
	check(CreateDeviceAndSwapchain());
	check(CreateRenderTargetView());
	check(CreateDepthStencilView());
	check(CreateDepthStencilState());
	check(CreateRasterizerState());
#undef check

	Viewport.Width = static_cast<float>(clientWidth);
	Viewport.Height = static_cast<float>(clientHeight);
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	return 0;
}

void D3DContext::Clear(const float clearColor[4], float clearDepth, uint8_t clearStencil) {
	d3dDeviceContext->ClearRenderTargetView(BackBufferRTV, clearColor);
	d3dDeviceContext->ClearDepthStencilView(d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

void D3DContext::Present() {
	d3dSwapChain->Present(vsync ? 1 : 0, 0);
}

void D3DContext::Cleanup() {
	SafeRelease( d3dDepthStencilView );
	SafeRelease( BackBufferRTV );
	SafeRelease( d3dDepthStencilBuffer );
	SafeRelease( d3dDepthStencilState );
	SafeRelease( d3dRasterizerState );
	SafeRelease( d3dSwapChain );
	SafeRelease( d3dDeviceContext );
	SafeRelease( d3dDevice );
}
