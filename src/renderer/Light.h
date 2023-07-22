#pragma once

#include "DirectXTemplatePCH.h"

#define MAX_LIGHTS 32

struct alignas(16) Light {
	union {
		DirectX::XMFLOAT3 pos;
		float _pad[4];
	};
	DirectX::XMFLOAT3 colour;
	float radius;
};
static_assert(sizeof(Light) == 8*4);

struct alignas(16) LightData {
	Light lights[MAX_LIGHTS];
	int numLights = 0;
};
static_assert(sizeof(LightData) == 1040);
