#pragma once

#include "PCH.h"

#include "D3DContext.h"

struct Shader {
	const void* bytecode;
	size_t bytecodeSize;
};

class VertexShader : public Shader {
public:
	VertexShader(D3DContext* d3dContext, const void* bytecode, size_t bytecodeSize);
	void Bind(D3DContext* d3dContext);
	ID3D11InputLayout* CreateInputLayout(D3DContext* d3dContext, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc);
	ID3D11VertexShader* vertexShader;
};

class PixelShader : public Shader {
public:
	PixelShader(D3DContext* d3dContext, const void* bytecode, size_t bytecodeSize);
	void Bind(D3DContext* d3dContext);
	ID3D11PixelShader* pixelShader;
};
