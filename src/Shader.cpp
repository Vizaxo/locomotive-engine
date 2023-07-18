#include "DirectXTemplatePCH.h"

#include "Shader.h"

VertexShader::VertexShader(const void* bytecode, size_t bytecodeSize, ID3D11VertexShader* vs)
	: Shader({ bytecode, bytecodeSize }), vs(vs)
{}
