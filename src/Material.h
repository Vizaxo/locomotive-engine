#pragma once

#include "DirectXTemplatePCH.h"

class Material {
public:
	Material(ID3D11PixelShader* pixelShader);
	~Material();

	ID3D11PixelShader* pixelShader = nullptr;
};
