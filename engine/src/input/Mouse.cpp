#include "PCH.h"

#include "Mouse.h"
#include "Application.h"

namespace Mouse {

int x = 0;
int y = 0;
int dx = 0;
int dy = 0;
v2i savedCursorPos;

MouseState mouseState = Free;

Mouse::Button buttonState = Mouse::Button::NONE;

void setMousePos(int _x, int _y) {
	dx += _x - x;
	dy += _y - y;
	x = _x;
	y = _y;
}

void setMouseButtonState(Mouse::Button newState) {
	buttonState = newState;
}

void buttonDown(Button b) { Application::mouseButtonDown(b); }
void buttonUp(Button b)   { Application::mouseButtonUp(b);   }

void lockCursorToWindow(PAL::WindowHandle* h) {
	assert(mouseState == Free);
	savedCursorPos = PAL::getCursorPos();
	PAL::mouseSetCapture(h);
	PAL::showCursor(false);

	mouseState = LockedToWindow;
}

void unlockCursorFromWindow(PAL::WindowHandle* h) {
	assert(mouseState == LockedToWindow);
	PAL::mouseReleaseCapture(h);
	PAL::showCursor(true);
	PAL::setCursorPosition(savedCursorPos);

	mouseState = Free;
}

void endTick() {
	dx = 0;
	dy = 0;
}

}