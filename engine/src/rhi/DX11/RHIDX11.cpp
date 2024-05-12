#include "PCH.h"
#include "RHIDX11.h"
#include "Constants.h"
#include "core/Assert.h"
#include "platform/Platform.h"
#include "platform/Windows/Windows.h"

using namespace DirectX;

#ifndef PLATFORM_WINDOWS
#error "DX11 only supported on Windows"
#endif

RHI createRHI(RefPtr<PAL::WindowHandle> h) {
	ASSERT(DirectX::XMVerifyCPUSupport(), "Failed to verify CPU DirectX support");
	HWND hWnd = h->hwnd;

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
	float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Format = RHI::BACK_BUFFER_FORMAT;
	swapChainDesc.BufferDesc.RefreshRate = { 0, 1 }; // QueryRefreshRate(clientWidth, clientHeight, vSync);
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
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

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;
	IDXGISwapChain* swapChain;
	D3D_FEATURE_LEVEL featureLevel;
	HRASSERT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &d3dDevice, &featureLevel,
		&d3dDeviceContext));

	return {std::move(d3dDevice), std::move(d3dDeviceContext), std::move(swapChain), featureLevel};
}

RHI::VertexShader RHI::createVertexShaderFromBytecode(RefPtr<u8> bytecode, size_t size) {
	ID3D11VertexShader* vertexShader;
	device->CreateVertexShader(bytecode.getRaw(), size, nullptr, &vertexShader);
	return { bytecode, size, nullptr, std::move(vertexShader) };
}

void RHI::setVertexShader(RefPtr<RHI::VertexShader> vs) {
	deviceContext->VSSetShader(vs->vertexShader.getRaw(), nullptr, 0);
}

void RHI::setPixelShader(RefPtr<RHI::PixelShader> ps) {
	deviceContext->PSSetShader(ps->pixelShader.getRaw(), nullptr, 0);
}

RHI::PixelShader RHI::createPixelShaderFromBytecode(RefPtr<u8> bytecode, size_t size) {
	ID3D11PixelShader* pixelShader;
	device->CreatePixelShader(bytecode.getRaw(), size, nullptr, &pixelShader);
	return { bytecode, size, nullptr, std::move(pixelShader) };
}

RHI::InputLayout RHI::createInputLayout(RefPtr<D3D11_INPUT_ELEMENT_DESC> descs, size_t count, RefPtr<VertexShader> vs) {
	OwningPtr<ID3D11InputLayout, true, ReleaseCOM> inputLayout = nullptr;
	device->CreateInputLayout(descs.getRaw(), count, vs->bytecode.getRaw(), vs->size, &inputLayout.getRaw());
	OwningPtr<D3D11_INPUT_ELEMENT_DESC, false, ArrayDelete> descsCpy = new D3D11_INPUT_ELEMENT_DESC[count]();
	memcpy(descsCpy.getRaw(), descs.getRaw(), count);
	return { std::move(descsCpy), inputLayout.getNonNull() };
}

void RHI::setVertexBuffer(RefPtr<RHI::VertexBuffer> vertexBuffer, uint slot) {
	uint offset = 0;
	deviceContext->IASetVertexBuffers(slot, 1, &vertexBuffer->gpu_vertexBuffer.getRaw(), &vertexBuffer->stride, &offset);
}

void RHI::setIndexBuffer(RefPtr<IndexBuffer> indexBuffer) {
	deviceContext->IASetIndexBuffer(indexBuffer->gpu_indexBuffer.getRaw(), indexBuffer->format, 0);
	deviceContext->IASetPrimitiveTopology(indexBuffer->primitiveTopology);
}

void RHI::RenderTargetView::clear(Colour clearColour) {
	rhi->deviceContext->ClearRenderTargetView(rtv.getRaw(), (float*)&clearColour);
}

void RHI::DepthStencilView::clear(float clearDepth, u8 clearStencil) {
	rhi->deviceContext->ClearDepthStencilView(depthStencilView.getRaw(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

RHI::RenderTargetView RHI::createBackBufferRTV() {
	ID3D11Texture2D* backBuffer;
	HRASSERT(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = BACK_BUFFER_FORMAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = {0};

	ID3D11RenderTargetView* backBufferRTV;
	device->CreateRenderTargetView(backBuffer, &rtvDesc, &backBufferRTV);

	return {std::move(backBufferRTV), this};
}