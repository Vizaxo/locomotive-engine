#pragma once

#include "PCH.h"
#include "core/Assert.h"
#include "types/Colour.h"
#include "types/Pointers.h"
#include "types/Types.h"
#include "platform/Platform.h"

struct RHI {
	struct Shader {
		RefPtr<u8> bytecode;
		size_t size;
		OwningPtr<ID3DBlob, true, ReleaseCOM> blob;
	};

	struct VertexShader : Shader {
		OwningPtr<ID3D11VertexShader, false, ReleaseCOM> vertexShader;
	};
	VertexShader createVertexShaderFromBytecode(RefPtr<u8> bytecode, size_t size);
	void setVertexShader(RefPtr<VertexShader> vs);

	struct PixelShader : Shader {
		OwningPtr<ID3D11PixelShader, false, ReleaseCOM> pixelShader;
	};
	PixelShader createPixelShaderFromBytecode(RefPtr<u8> bytecode, size_t size);
	void setPixelShader(RefPtr<PixelShader> ps);

	struct InputLayout {
		OwningPtr<D3D11_INPUT_ELEMENT_DESC, false, ArrayDelete> descs;
		OwningPtr<ID3D11InputLayout, false, ReleaseCOM> gpu_inputLayout;
	};
	InputLayout createInputLayout(RefPtr<D3D11_INPUT_ELEMENT_DESC> descs, size_t count, RefPtr<VertexShader> vs);
	void setInputLayout(RefPtr<InputLayout> inputLayout);

	struct VertexBuffer {
		OwningPtr<ID3D11Buffer, false, ReleaseCOM> gpu_vertexBuffer;
		uint stride;
	};
	template <typename T>
	VertexBuffer createVertexBuffer(RefPtr<T> verts, size_t count) {
		uint stride = sizeof(T);
		D3D11_BUFFER_DESC vertexBufferDesc = {};

		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = count * stride;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA resourceData = {};
		resourceData.pSysMem = verts.getRaw();

		ID3D11Buffer* vertexBuffer;
		HRASSERT(device->CreateBuffer(&vertexBufferDesc, &resourceData, &vertexBuffer));

		return { std::move(vertexBuffer), stride };
	}
	void setVertexBuffer(RefPtr<VertexBuffer> vertexBuffer, uint slot);

	struct IndexBuffer {
		OwningPtr<ID3D11Buffer, false, ReleaseCOM> gpu_indexBuffer;
		size_t indices;
		DXGI_FORMAT format;
		D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
	};
	void setIndexBuffer(RefPtr<IndexBuffer> indexBuffer);

	template <typename T>
	IndexBuffer createIndexBuffer(RefPtr<T> indices, size_t count, D3D11_PRIMITIVE_TOPOLOGY primitiveTopology) {
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.ByteWidth = count * sizeof(T);
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		DXGI_FORMAT format;
		switch (sizeof(T)) {
		case 1: format = DXGI_FORMAT_R8_UINT; break;
		case 2: format = DXGI_FORMAT_R16_UINT; break;
		case 4: format = DXGI_FORMAT_R32_UINT; break;
		default: ASSERT(false, "Index buffer format must be 1, 2, or 4 bytes. Got %d bytes.", sizeof(T));
		}
		D3D11_SUBRESOURCE_DATA resourceData = {};
		resourceData.pSysMem = indices.getRaw();

		ID3D11Buffer* indexBuffer;
		HRASSERT(device->CreateBuffer(&indexBufferDesc, &resourceData, &indexBuffer));

		return {std::move(indexBuffer), count, format, primitiveTopology};
	}

	struct RenderTargetView {
		OwningPtr<ID3D11RenderTargetView, false, ReleaseCOM> rtv;
		RefPtr<RHI> rhi;

		void clear(Colour clearColour);
	};
	RHI::RenderTargetView createBackBufferRTV();

	struct DepthStencilView {
		OwningPtr<ID3D11DepthStencilView, false, ReleaseCOM> depthStencilView;
		DXGI_FORMAT format;
		RefPtr<RHI> rhi;

		void clear(float clearDepth, u8 clearStencil);
	};

	struct ConstantBuffer {
		OwningPtr<ID3D11Buffer, true, ReleaseCOM> gpu_constantBuffer = nullptr;
		u32 size;
	};
	template <typename T> ConstantBuffer createConstantBuffer(T data) {
		OwningPtr<ID3D11Buffer, true, ReleaseCOM> buf = nullptr;
		u32 size = alignTo<u32>(sizeof(T), 16);

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = size;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = &data;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRASSERT(device->CreateBuffer(&desc, &subresourceData, &buf.getRaw()));
		return ConstantBuffer{ std::move(buf), size };
	}

	void present() {
		swapChain->Present(vsync, 0);
	}

	void PSsetConstantBuffer(u32 slot, RefPtr<ConstantBuffer> cb);
	void VSsetConstantBuffer(u32 slot, RefPtr<ConstantBuffer> cb);

	static const DXGI_FORMAT BACK_BUFFER_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
	OwningPtr<ID3D11Device, false, ReleaseCOM> device;
	OwningPtr<ID3D11DeviceContext, false, ReleaseCOM> deviceContext;
	OwningPtr<IDXGISwapChain, false, ReleaseCOM> swapChain;
	D3D_FEATURE_LEVEL featureLevel;
	bool vsync = true;

	static const inline u32 CONSTANT_BUFFER_COUNT = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT;
};

OwningPtr<RHI> createRHI(RefPtr<PAL::WindowHandle> h);
