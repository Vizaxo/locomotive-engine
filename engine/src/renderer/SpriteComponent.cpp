#include "PCH.h"

#include "SpriteComponent.h"
#include "Material.h"
#include "types/Pointers.h"
#include "rhi/RHI.h"

#include "generated/SpriteVS.h"
#include "generated/SpritePS.h"

RefPtr<Material, true> createSpriteMaterial(RefPtr<RHI> rhi, v2f windowSize) {
	RHI::VertexShader vs = rhi->createVertexShaderFromBytecode((u8*)spriteVS, sizeof(spriteVS));
	RHI::PixelShader ps = rhi->createPixelShaderFromBytecode((u8*)spritePS, sizeof(spritePS));
	OwningPtr<Material> mat = new Material({std::move(vs), std::move(ps)});

	mat->constantBuffers[0] = rhi->createConstantBuffer(windowSize);
	mat->constantBuffers[1] = rhi->createConstantBuffer(SpriteCB{});

	return materialManager.registerResource(sID("spriteMaterial"), std::move(mat));
}
