#pragma once

#include "PCH.h"

#include <vector>
#include "StaticMeshComponent.h"
#include "SpriteComponent.h"
#include "Light.h"
#include "types/Array.h"
#include "types/Matrix.h"
#include "Camera.h"

struct Scene {
	Array<StaticMeshComponent> objects;

	LightData lightData;
	Camera::Camera camera;
};
