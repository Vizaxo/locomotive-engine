#pragma once

#include "DirectXTemplatePCH.h"
#include "Texture2D.h"
#include <vector>

class Material {
public:
	Material(ID3D11PixelShader* pixelShader);
	~Material();
	Material* setTexture(D3DContext* d3dContext, Texture2D* texture);

	ID3D11PixelShader* pixelShader = nullptr;
	Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	ID3D11SamplerState* samplerState;
};
