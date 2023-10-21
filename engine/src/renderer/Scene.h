#pragma once

#include "PCH.h"

#include "Object.h"
#include "Light.h"

class Scene {
public:
	std::vector<Object> objects;
	LightData lightData;
	DirectX::XMVECTOR eyePosition;
	DirectX::XMVECTOR lookDirection;
	DirectX::XMVECTOR worldRight();
	DirectX::XMMATRIX localToWorld();
	DirectX::XMVECTOR worldUp = DirectX::XMVectorSet(0, 1, 0, 0);
};
