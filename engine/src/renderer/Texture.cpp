#include "PCH.h"

#include "Texture.h"

RefPtr<RHI::Texture2D, true> Texture::loadTextureFromFile(RefPtr<RHI> rhi, std::string path) {
	if (auto tex = textureManager.get(sID(path.c_str()))) {
		return tex;
	}

	v2i size;
	i32 stride;
	OwningPtr<u8, true, StbiImageFree> stbData = stbi_load(path.c_str(), &size.x, &size.y, &stride, 0);
	RefPtr<u8, true> data = stbData;
	if (!data)
		return nullptr;

	//TODO: find more reliable method to get pixel format
	RHICommon::PixelFormat pf;
	OwningPtr<u8, true, FreeMalloc> newData = nullptr;
	switch (stride) {
	case 3:
		pf = RHICommon::R8G8B8A8;
		// Extend to 4-component format
		newData = (u8*)malloc(4 * size.x * size.y);
		for (int x = 0; x < size.x; ++x) {
			for (int y = 0; y < size.y; ++y) {
				newData.getRaw()[(size.x * y + x) * 4 + 0] = stbData.getRaw()[(size.x * y + x) * stride + 0];
				newData.getRaw()[(size.x * y + x) * 4 + 1] = stbData.getRaw()[(size.x * y + x) * stride + 1];
				newData.getRaw()[(size.x * y + x) * 4 + 2] = stbData.getRaw()[(size.x * y + x) * stride + 2];
				newData.getRaw()[(size.x * y + x) * 4 + 3] = 0;
			}
		}
		data = newData;
		stride = 4;
		break;
	case 4: pf = RHICommon::R8G8B8A8; break;
	case 12: pf = RHICommon::R32G32B32; break;
	case 16: pf = RHICommon::R32G32B32A32; break;
	default: ASSERT(false, "Unsupported texture stride %d", stride);
	}

	OwningPtr<RHI::Texture2D> texture = rhi->createTexture(pf, data.getNonNull(), stride, size);
	texture->path = path;
	return textureManager.registerResource(sID(path.c_str()), std::move(texture));
}

static ResourceManager<RHI::Texture2D> Texture::textureManager;
