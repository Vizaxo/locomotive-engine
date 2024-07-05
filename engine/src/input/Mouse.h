#include "PCH.h"

#pragma once

#include "platform/Platform.h"
#include "events/EventQueue.h"

namespace Mouse {

enum class Button : u32 {
	NONE = 0x0,
	M1 = 0x1,
	M2 = 0x2,
	M3 = 0x4,
	M4 = 0x8,
	M5 = 0x10,
};

enum MouseState {
	LockedToWindow,
	Free,
};

extern int x;
extern int y;
extern int dx;
extern int dy;
extern Button buttonState;

void setMousePos(int x, int y);
void setMouseButtonState(Mouse::Button newState);
void buttonDown(Button b);
void buttonUp(Button b);
void unlockCursorFromWindow(PAL::WindowHandle* h);
void lockCursorToWindow(PAL::WindowHandle* h);
void endTick();
void handleMouseButtonEvent(EventQueue::Event::MouseButtonEvent ev);
void handleMouseMoveEvent(EventQueue::Event::MouseMoveEvent ev);

}
