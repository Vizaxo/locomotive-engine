#include "PCH.h"

#include "SpriteComponent.h"

#include "ConstantBuffers.h"
#include "Material.h"
#include "types/Pointers.h"
#include "rhi/RHI.h"

#include "generated/SpritePS.h"

RefPtr<Material, true> createSpriteMaterial(RefPtr<RHI> rhi, v2f windowSize) {
	RHI::VertexShader vs = rhi->createVertexShaderFromFile(ENGINE_SHADER("SpriteVS"), "main");
	RHI::PixelShader ps = rhi->createPixelShaderFromFile(ENGINE_SHADER("SpritePS"), "main");
	OwningPtr<Material> mat = new Material({std::move(vs), std::move(ps)});

	mat->constantBuffers[CB::View] = rhi->createConstantBuffer(CB::ViewCB{});
	mat->constantBuffers[CB::Sprite] = rhi->createConstantBuffer(SpriteComponentCB{});
	mat->constantBuffers[CB::SpriteSheet] = rhi->createConstantBuffer(SpriteSheetCB{});

	return materialManager.registerResource(sID("spriteMaterial"), std::move(mat));
}

SpriteComponent SpriteComponent::createSpriteComponent(RefPtr<RHI> rhi, SpriteComponentCB cbData) {
	return SpriteComponent({
		cbData,
		true,
	});

}
