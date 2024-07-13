#pragma once

#include "PCH.h"

#include "rhi/RHI.h"
#include "resource-manager/ResourceManager.h"

struct Material {
	RHI::VertexShader vertexShader;
	RHI::PixelShader pixelShader;
	//RHI::InputLayout inputLayout;

	RHI::ConstantBuffer constantBuffers[RHI::CONSTANT_BUFFER_COUNT] = {};
};

extern ResourceManager<Material> materialManager;
