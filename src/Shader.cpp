#include "DirectXTemplatePCH.h"

#include "Shader.h"

VertexShader::VertexShader(D3DContext* d3dContext, const void* bytecode, size_t bytecodeSize)
	: Shader({ bytecode, bytecodeSize })
{
	HRASSERT(d3dContext->d3dDevice->CreateVertexShader(bytecode, bytecodeSize, nullptr, &vertexShader));
}

ID3D11InputLayout* VertexShader::CreateInputLayout(D3DContext* d3dContext, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc)
{
	ID3D11InputLayout* layout;
	HRASSERT(d3dContext->d3dDevice->CreateInputLayout(inputDesc.data(), inputDesc.size(), bytecode, bytecodeSize, &layout));
	return layout;
}

PixelShader::PixelShader(D3DContext* d3dContext, const void* bytecode, size_t bytecodeSize)
	: Shader({ bytecode, bytecodeSize })
{
	HRASSERT(d3dContext->d3dDevice->CreatePixelShader(bytecode, bytecodeSize, nullptr, &pixelShader));
}

void VertexShader::Bind(D3DContext* d3dContext)
{
	d3dContext->d3dDeviceContext->VSSetShader(vertexShader, nullptr, 0);
}

void PixelShader::Bind(D3DContext* d3dContext)
{
	d3dContext->d3dDeviceContext->PSSetShader(pixelShader, nullptr, 0);
}
