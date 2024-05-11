#pragma once

#include "PCH.h"
#include "types/Pointers.h"
#include "types/Types.h"
#include "platform/Platform.h"

struct RHI {

	OwningPtr<ID3D11Device, false, ReleaseCOM> device;
	OwningPtr<ID3D11DeviceContext, false, ReleaseCOM> deviceContext;
	OwningPtr<IDXGISwapChain, false, ReleaseCOM> swapChain;
	D3D_FEATURE_LEVEL featureLevel;

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

	struct VertexBuffer {
		OwningPtr<ID3D11Buffer> gpu_vertexBuffer;
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
		OwningPtr<ID3D11Buffer> gpu_indexBuffer;
	};
};

RHI createRHI(RefPtr<PAL::WindowHandle> h);
