#pragma once

#include "PCH.h"

#include "types/Types.h"
#include "events/EventQueue.h"

namespace Keyboard {
enum class Key : u32 {
	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Backtick,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	Unknown,
};

extern std::set<Keyboard::Key> keysDown;

std::string toString(Key k);
void keyDown(Keyboard::Key k);
void keyUp(Keyboard::Key k);
void handleKeyboardEvent(EventQueue::Event::KeyboardEvent ev);

}
