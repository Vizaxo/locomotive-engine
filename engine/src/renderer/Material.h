#pragma once

#include "PCH.h"

#include "rhi/RHI.h"
#include "resource-manager/ResourceManager.h"

struct Material {
	static constexpr u32 MAX_TEXTURES = 8;
	RHI::VertexShader vertexShader;
	RHI::PixelShader pixelShader;
	RefPtr<RHI::Texture2D, true> textures[MAX_TEXTURES] = {};

	RHI::ConstantBuffer constantBuffers[RHI::CONSTANT_BUFFER_COUNT] = {};

	static RefPtr<Material, true> createMaterial(RefPtr<RHI> rhi,
		StringId name, std::wstring vsFile, std::string vsEntrypoint, std::wstring psFile, std::string psEntrypoint);
	static RefPtr<Material, true> createMaterial(RefPtr<RHI> rhi,
		StringId name, std::wstring file, std::string vsEntrypoint, std::string psEntrypoint);
	static void createBasicMaterials(RefPtr<RHI> rhi);
};

extern ResourceManager<Material> materialManager;
