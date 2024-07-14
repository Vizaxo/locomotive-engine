#include "PCH.h"

#include "Texture.h"

RefPtr<RHI::Texture2D, true> Texture::loadTextureFromFile(RefPtr<RHI> rhi, std::string path) {
	if (auto tex = textureManager.get(sID(path.c_str()))) {
		return tex;
	}

	v2i size;
	i32 stride;
	OwningPtr<u8, false, StbiImageFree> data = stbi_load(path.c_str(), &size.x, &size.y, &stride, 0);

	//TODO: find more reliable method to get pixel format
	RHICommon::PixelFormat pf;
	switch (stride) {
	case 4: pf = RHICommon::R8G8B8A8; break;
	case 12: pf = RHICommon::R32G32B32; break;
	case 16: pf = RHICommon::R32G32B32A32; break;
	default: ASSERT(false, "Unsupported texture stride %d", stride);
	}

	OwningPtr<RHI::Texture2D> texture = rhi->createTexture(pf, data, stride, size);
	texture->path = path;
	return textureManager.registerResource(sID(path.c_str()), std::move(texture));
}

static ResourceManager<RHI::Texture2D> Texture::textureManager;
