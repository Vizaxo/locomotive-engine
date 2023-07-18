#include "DirectXTemplatePCH.h"

#include "Shader.h"

VertexShader::VertexShader(D3DContext* d3dContext, const void* bytecode, size_t bytecodeSize)
	: Shader({ bytecode, bytecodeSize })
{
	HRASSERT(d3dContext->d3dDevice->CreateVertexShader(bytecode, bytecodeSize, nullptr, &vertexShader));
}

PixelShader::PixelShader(D3DContext* d3dContext, const void* bytecode, size_t bytecodeSize)
	: Shader({ bytecode, bytecodeSize })
{
	HRASSERT(d3dContext->d3dDevice->CreatePixelShader(bytecode, bytecodeSize, nullptr, &pixelShader));
}

