#pragma once

#include "PCH.h"

#include <vector>
#include "StaticMeshComponent.h"
#include "Light.h"

class Scene {
public:
	//std::vector<StaticMeshComponent> objects;
	LightData lightData;
	DirectX::XMVECTOR eyePosition;
	DirectX::XMVECTOR lookDirection;
	DirectX::XMVECTOR worldRight();
	DirectX::XMMATRIX localToWorld();
	DirectX::XMVECTOR worldUp = DirectX::XMVectorSet(0, 1, 0, 0);
};
