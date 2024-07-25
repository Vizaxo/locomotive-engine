#pragma once

#include "types/Types.h"
#include "types/Vector.h"
#include <optional>

namespace Keyboard {
	enum Key : u32;
}

namespace Mouse {
	enum class Button : u32;
}

namespace EventQueue {

//TODO: better to have this as closed type or extendible?
enum class EventType {
	Keyboard,
	MouseButton,
	MouseMove,
	Frame,
};

enum class ButtonEventType { Down, Up };

struct Event {
	EventType type;

	struct KeyboardEvent {
		Keyboard::Key k;
		ButtonEventType type;
	};
	struct MouseButtonEvent {
		Mouse::Button b;
		ButtonEventType type;
	};
	struct MouseMoveEvent {
		v2i pos;
	};
	struct FrameEvent {
		enum class Ty { Begin, End };
		u32 frameNumber;
		Ty type;
	};
	union {
		KeyboardEvent keyboardEvent;
		MouseButtonEvent mouseButtonEvent;
		MouseMoveEvent mouseMoveEvent;
		FrameEvent frameEvent;
	};
};

void pushEvent(Event ev);
std::optional<Event> popEvent();
void frameBeginEvent(i32 frameNumber);
void frameEndEvent(i32 frameNumber);
void mouseMoveEvent(v2i pos);
void mouseButtonDownEvent(Mouse::Button b);
void mouseButtonUpEvent(Mouse::Button b);
void keyDownEvent(Keyboard::Key k);
void keyUpEvent(Keyboard::Key k);

void a();
}

