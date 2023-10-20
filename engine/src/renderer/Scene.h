#pragma once

#include "PCH.h"

#include "Object.h"
#include "Light.h"

class Scene {
public:
	std::vector<Object> objects;
	LightData lightData;
	DirectX::XMVECTOR eyePosition;
	DirectX::XMVECTOR focusPoint;
};
