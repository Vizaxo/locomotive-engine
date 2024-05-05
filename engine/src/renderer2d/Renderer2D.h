#pragma once

#include "types/Box.h"
#include "types/Colour.h"
#include "types/Pointers.h"
#include "types/Vector.h"

#include <vector>

struct Circle {
	v2f pos;
	float radius;

	// rendering
	Colour col;

	// physics
	v2f velocity;
	float mass;
};

struct Scene2D {
	std::vector<Circle> circles;
	Box2D viewport;

	Scene2D() {
		circles.emplace_back(Circle({{100, 100}, 20, {1, 1, 0}}));
		circles.emplace_back(Circle({{110, 200}, 20, {1, 1, 0}, {0.0, 5.0}}));
		viewport = {{-100, -100}, {100, 100}};
	}
};

namespace Renderer2D {

void init(HWND hwnd);
void tickPhysics(RefPtr<Scene2D> scene, float deltaTime);
void renderScene(float deltaTime);

}
