#pragma once

#include "Object.h"
#include "Light.h"
#include <vector>

class Scene {
public:
	std::vector<Object> objects;
	LightData lightData;
};
