#pragma once

#include "PCH.h"

#include <vector>
#include "StaticMeshComponent.h"
#include "SpriteComponent.h"
#include "Light.h"
#include "types/Array.h"

struct Scene {
	Array<StaticMeshComponent> objects;

	LightData lightData;

	//TODO: refactor camera info
	v2f eyePosition = {0.f, 0.f};
	DirectX::XMVECTOR lookDirection;
	DirectX::XMVECTOR worldRight();
	DirectX::XMMATRIX localToWorld();
	DirectX::XMVECTOR worldUp = DirectX::XMVectorSet(0, 1, 0, 0);
	float camZoom = 1.f;
};
