#include "PCH.h"

#pragma once

namespace Mouse {

enum Button {
	NONE = 0x0,
	M1 = 0x1,
	M2 = 0x2,
	M3 = 0x4,
	M4 = 0x8,
	M5 = 0x10,
};

extern int x;
extern int y;
extern Button buttonState;

void setMousePos(int x, int y);
void setMouseButtonState(Mouse::Button newState);

}
