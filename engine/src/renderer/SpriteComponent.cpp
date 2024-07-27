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

RHI::InputLayout SpriteComponent::createSpriteInputLayout(RefPtr<RHI> rhi) {
	static const u32 SPRITE_INPUT_DESC_COUNT = 2;
	D3D11_INPUT_ELEMENT_DESC spriteInputLayoutDescs[SPRITE_INPUT_DESC_COUNT];
	spriteInputLayoutDescs[0].SemanticName = "POSITION";
	spriteInputLayoutDescs[0].SemanticIndex = 0;
	spriteInputLayoutDescs[0].Format = DXGI_FORMAT_R32G32_FLOAT;
	spriteInputLayoutDescs[0].InputSlot = 0;
	spriteInputLayoutDescs[0].AlignedByteOffset = 0;
	spriteInputLayoutDescs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	spriteInputLayoutDescs[0].InstanceDataStepRate = 0;
	spriteInputLayoutDescs[1].SemanticName = "TEXCOORD";
	spriteInputLayoutDescs[1].SemanticIndex = 0;
	spriteInputLayoutDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	spriteInputLayoutDescs[1].InputSlot = 0;
	spriteInputLayoutDescs[1].AlignedByteOffset = sizeof(v2f);
	spriteInputLayoutDescs[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	spriteInputLayoutDescs[1].InstanceDataStepRate = 0;
	return rhi->createInputLayout(spriteInputLayoutDescs, SPRITE_INPUT_DESC_COUNT, &materialManager.get(sID("spriteMaterial"))->vertexShader);
}

SpriteComponent SpriteComponent::createSpriteComponent(RefPtr<RHI> rhi, SpriteComponentCB cbData) {
	return SpriteComponent({
		cbData,
		true,
	});

}
