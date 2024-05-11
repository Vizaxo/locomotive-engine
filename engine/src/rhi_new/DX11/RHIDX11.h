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
		int slot; //do I need to store this here?
		OwningPtr<ID3D11Buffer> gpu_vertexBuffer;
	};

	struct IndexBuffer {
		OwningPtr<ID3D11Buffer> gpu_indexBuffer;
	};
};

RHI createRHI(RefPtr<PAL::WindowHandle> h);
