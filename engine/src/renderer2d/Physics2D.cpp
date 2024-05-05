#include "PCH.h"

#include "Renderer2D.h"

#include "types/Pointers.h"
#include "types/Vector.h"

void Renderer2D::tickPhysics(RefPtr<Scene2D> scene, float deltaTime) {
	for (int i = 0; i < scene->circles.size(); ++i) {
		Circle& c1 = scene->circles[i];
		for (int j = i+1; j < scene->circles.size(); ++j) {
			//detect collision
			Circle& c2 = scene->circles[j];

			float r = (c1.radius + c2.radius);
			if (lenSquared(c1.pos - c2.pos) <= r*r) {
				// collision

				// temp
				static const float collisionFactor = 1.0;
				c1.velocity = (normalize(c2.pos - c1.pos) * collisionFactor) / c1.mass;
				c2.velocity = (normalize(c1.pos - c2.pos) * collisionFactor) / c2.mass;
			}
		}

		const v2f g = {0.0, 5.0};
		c1.velocity = c1.velocity + g * deltaTime;

		c1.pos = c1.pos + c1.velocity;
	}
}