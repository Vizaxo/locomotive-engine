#pragma once
#include "types/Matrix.h"
#include "types/Vector.h"

struct TransformComponent {
	v3 pos = {0.0, 0.0, 0.0};
	v3 rot = {0.0, 0.0, 0.0};
	v3 scale = {1.0, 1.0, 1.0};
	m44 getMatrix();
};

inline m44 TransformComponent::getMatrix() {
	return translate(pos) * euler(rot) * ::scale(scale);
}
