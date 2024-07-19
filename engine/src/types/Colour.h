#pragma once

#include "Types.h"
#include "Vector.h"

struct Colour {
	u8 r, g, b, a;

	static const Colour BLACK, WHITE, RED, GREEN, BLUE;

	operator v4f() {
		return {(float)r, (float)g, (float)b, (float)a};
	}
};

