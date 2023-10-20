#pragma once

#include "PCH.h"

#define MAX_LIGHTS 32

struct alignas(16) PointLight {
	union {
		DirectX::XMFLOAT3 pos;
		float _pad[4];
	};
	DirectX::XMFLOAT3 colour;
	float radius;
};
static_assert(sizeof(PointLight) == 8*4);

struct DirectionalLight {
	union {
		DirectX::XMFLOAT3 direction;
		float _pad[4];
	};
	DirectX::XMFLOAT3 colour;
};

struct alignas(16) LightData {
	PointLight pointLights[MAX_LIGHTS];
	DirectionalLight directionalLight;
	int numPointLights = 0;
};
