#pragma once

#include "PCH.h"

#include "rhi/RHI.h"

class Material {
public:
	Material(PixelShader& pixelShader);
	Material* setTexture(D3DContext* d3dContext, Texture2D* texture);

	PixelShader& pixelShader;
	Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
};
