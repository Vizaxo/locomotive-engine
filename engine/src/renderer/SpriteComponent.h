#pragma once

#include "types/Vector.h"
#include "types/Pointers.h"
#include "Material.h"
#include "rhi/RHI.h"

struct SpriteComponent {
	v2f pos;
	v2f size;
	RefPtr<RHI::Texture2D> texture;
	RHI::InputLayout inputLayout;

	static SpriteComponent createSpriteComponent(RefPtr<RHI> rhi, v2f pos, v2f size, RefPtr<RHI::Texture2D> texture);
};

struct SpriteCB {
	v2f pos;
	v2f size;
};

RefPtr<Material, true> createSpriteMaterial(RefPtr<RHI> rhi, v2f windowSize);
