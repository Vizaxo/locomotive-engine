#include "DirectXTemplatePCH.h"

#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h":

Texture2D::Texture2D(D3DContext* d3dContext, char* path) {
	int width, height, n;
	unsigned char* textureMem = stbi_load(path, &width, &height, &n, 4);
	assert(textureMem);

	D3D11_SUBRESOURCE_DATA data = { textureMem, (UINT)width * 4, 0 };
	DXGIFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	D3D11_TEXTURE2D_DESC texture2DDesc = {
		width, height, 1, 1, DXGIFormat, {1, 0},
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0
	};
	HRASSERT(d3dContext->d3dDevice->CreateTexture2D(&texture2DDesc, &data, &d3dTexture2D));

	stbi_image_free(textureMem);
}

Texture2D::~Texture2D() {
	SafeRelease(d3dTexture2D);
}
