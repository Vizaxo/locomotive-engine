#pragma once

#include "rhi/RHI.h"
#include "stb/stb_image.h"
#include "resource-manager/ResourceManager.h"

namespace Texture {
	RefPtr<RHI::Texture2D, true> loadTextureFromFile(RefPtr<RHI> rhi, std::string path);

	extern ResourceManager<RHI::Texture2D> textureManager;
}

class StbiImageFree {
public:
	void operator()(void** data) {stbi_image_free((u8*)*data);}
};
