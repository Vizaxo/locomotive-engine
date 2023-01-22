#pragma once

#include "DirectXTemplatePCH.h"
#include "D3DContext.h"

class Texture2D {
public:
	Texture2D(D3DContext* d3dContext, char* path);
	~Texture2D();

	ID3D11Texture2D* d3dTexture2D;
	DXGI_FORMAT DXGIFormat;
};
