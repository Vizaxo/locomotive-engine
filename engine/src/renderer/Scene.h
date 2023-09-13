#pragma once

#include "DirectXTemplatePCH.h"

#include "Object.h"
#include "Light.h"
#include <vector>

class Scene {
public:
	std::vector<Object> objects;
	LightData lightData;
	DirectX::XMVECTOR eyePosition;
	DirectX::XMVECTOR focusPoint;
};
