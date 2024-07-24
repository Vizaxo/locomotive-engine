#pragma once

#include "types/Vector.h"
#include "types/Pointers.h"
#include "SpriteSheet.h"
#include "Material.h"
#include "rhi/RHI.h"

struct SpriteComponentCB {
	v2f pos;
	v2f size;
	v2f index;
	v2f numTiles = {1, 1};
};

struct SpriteComponent {
	SpriteComponentCB cbData;
	bool enabled = false;

	static SpriteComponent createSpriteComponent(RefPtr<RHI> rhi, SpriteComponentCB cbData);
	static RHI::InputLayout createSpriteInputLayout(RefPtr<RHI> rhi);
};

RefPtr<Material, true> createSpriteMaterial(RefPtr<RHI> rhi, v2f windowSize);
