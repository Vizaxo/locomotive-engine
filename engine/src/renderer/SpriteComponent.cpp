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

SpriteComponent SpriteComponent::createSpriteComponent(RefPtr<RHI> rhi, v2f pos, v2f size, RefPtr<RHI::Texture2D> texture) {
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
	RHI::InputLayout spriteInputLayout = rhi->createInputLayout(spriteInputLayoutDescs, SPRITE_INPUT_DESC_COUNT, &materialManager.get(sID("spriteMaterial"))->vertexShader);

	return SpriteComponent({
		pos,
		size,
		texture,
		std::move(spriteInputLayout),
		true,
	});

}
