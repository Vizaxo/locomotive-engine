#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"

struct Shader {
	const void* bytecode;
	size_t bytecodeSize;
};

class VertexShader : public Shader {
public:
	VertexShader(D3DContext* d3dContext, const void* bytecode, size_t bytecodeSize);
	ID3D11VertexShader* vertexShader;
};

class PixelShader : public Shader {
public:
	PixelShader(D3DContext* d3dContext, const void* bytecode, size_t bytecodeSize);
	ID3D11PixelShader* pixelShader;
};
