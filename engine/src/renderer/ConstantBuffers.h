#pragma once

#include "types/Matrix.h"
#include "types/Types.h"

namespace CB {
enum ConstantBufferSlot : u8 {
	View = 0,
	Sprite = 1,
	SpriteSheet = 2,
};

struct ViewCB {
	v2f windowSize;
	v2f camPos;
	m44 mvp;
	float zoomScale;
};
}
