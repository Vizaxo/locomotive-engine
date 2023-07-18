#pragma once

#include "DirectXTemplatePCH.h"

struct Shader {
	const void* bytecode;
	size_t bytecodeSize;
};

class VertexShader : public Shader {
public:
	VertexShader(const void* bytecode, size_t bytecodeSize, ID3D11VertexShader* vs);
	ID3D11VertexShader* vs;
};
