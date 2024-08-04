#pragma once
#include "types/Matrix.h"
#include "types/Vector.h"

struct TransformComponent {
	v3 pos;
	v3 rot;
	v3 scale;
	m44 getMatrix();
};

inline m44 TransformComponent::getMatrix() {
	return translate(pos) * rotZ(rot.z) * rotY(rot.y) * rotX(rot.x);
}
