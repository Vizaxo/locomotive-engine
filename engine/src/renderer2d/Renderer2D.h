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

static inline float randf() {
	return 2.0*((float)rand() / (float)(RAND_MAX)) - 1.0;
}

struct Scene2D {
	std::vector<Circle> circles;
	Box2D viewport;

	Scene2D() {
		circles.emplace_back(Circle({{100, 100}, 20, {1, 1, 0}, 1.0}));
		circles.emplace_back(Circle({{110, 200}, 20, {1, 1, 0}, {0.0, 5.0}, 1.0}));

		for (int i = 0; i < 20; ++i) {
			circles.emplace_back(Circle({ {randf()*200 + 400, randf()*200 + 400}, 20, {1, 1, 0}, v2f({(float)(randf()*5.0), (float)(randf()*5.0)}), 1.0 }));
		}
		viewport = {{-100, -100}, {100, 100}};
	}
};

namespace Renderer2D {

void init(HWND hwnd);
void tickPhysics(RefPtr<Scene2D> scene, float deltaTime);
void renderScene(float deltaTime);

}
