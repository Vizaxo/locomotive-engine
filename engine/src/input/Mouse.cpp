#include "PCH.h"

#include "Mouse.h"

namespace Mouse {

int x = 0;
int y = 0;
Mouse::Button buttonState = Mouse::Button::NONE;

void setMousePos(int _x, int _y) {
	x = _x;
	y = _y;
}

void setMouseButtonState(Mouse::Button newState) {
	buttonState = newState;
}

}