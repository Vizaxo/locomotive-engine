#pragma once

#include "PCH.h"

#include <vector>
#include "StaticMeshComponent.h"
#include "Light.h"

struct Scene {
	OwningPtr<StaticMeshComponent, true, ArrayDelete> objects = nullptr;
	size_t obj_count;
	LightData lightData;
	DirectX::XMVECTOR eyePosition;
	DirectX::XMVECTOR lookDirection;
	DirectX::XMVECTOR worldRight();
	DirectX::XMMATRIX localToWorld();
	DirectX::XMVECTOR worldUp = DirectX::XMVectorSet(0, 1, 0, 0);
};
