#pragma once

#include "Object.h"
#include "D3DContext.h"
#include <vector>

class Scene {
public:
	std::vector<IObject*> objects;

	void RenderScene(D3DContext* d3dContext, float deltaTime);
};
