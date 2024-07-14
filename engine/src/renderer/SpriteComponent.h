#pragma once

#include "types/Vector.h"
#include "types/Pointers.h"
#include "Material.h"
#include "rhi/RHI.h"

struct SpriteComponent {
	v2f pos;
	v2f size;
	v3f color; //TODO: replace with texture
	RHI::InputLayout inputLayout;
};

RefPtr<Material, true> createSpriteMaterial(RefPtr<RHI> rhi);
