#pragma once
#include "types/Matrix.h"
#include "types/Vector.h"

namespace Camera {

struct Camera3D {
	v3 pos;
	//v3 lookDirection = {0., 0., 1. };
	//v3 worldUp = {0., 1., 0. };
	v3 rot;
	float fov = 45.;
	bool customProj = false;
	m44 projection;

	v3 worldRight();
	m44 view(); // world space -> camera space
	m44 viewProj(float aspect, float n, float f);
	void setCustomProj(m44 proj);
};

struct Camera2D {
	v2f eyePosition = {0.f, 0.f};
	float camZoom = 1.f;
};

enum Type {Cam3D, Cam2D};

struct Camera {
	struct {
		Camera3D cam3d;
		Camera2D cam2d;
	};
	Type type; // TODO: do we need both? How do we handle 2D and 3D scenes?
};
}
