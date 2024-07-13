#include "PCH.h"

#include "EngineMain.h"
#include "Keyboard.h"
#include "core/Assert.h"
#include "types/Types.h"
#include <queue>

namespace Keyboard {

std::set<Keyboard::Key> keysDown = {};

std::string toString(Key k) {
	std::string s;
	char c[2];
	if (k >= Key::NUM0 && k <= Key::NUM9) {
		c[0] = (static_cast<char>(k) - static_cast<char>(Keyboard::Key::NUM0) + '0');
	} else if (k >= Key::A && k <= Key::Z) {
		c[0] = (static_cast<char>(k) - static_cast<char>(Keyboard::Key::A) + 'A');
	} else {
		ASSERT(false, "Unrecognised key %d", (u32)k);
	}
	c[1] = '\0';
	return s.append(c);
}

void keyDown(Keyboard::Key k) {
	keysDown.insert(k);
}

void keyUp(Keyboard::Key k) {
	keysDown.erase(k);
}

void onButtonPress(Keyboard::Key k) {
	switch (k) {
	case Keyboard::Key::R:
		if (Engine::renderdoc) Engine::renderdoc->captureFrame();
		break;
	}
}

void handleKeyboardEvent(EventQueue::Event::KeyboardEvent ev) {
	switch (ev.type) {
	case EventQueue::ButtonEventType::Down:
		keyDown(ev.k);
		onButtonPress(ev.k);
		break;
	case EventQueue::ButtonEventType::Up:
		keyDown(ev.k);
		break;
	default:
		ASSERT(false, "Invalid keyboard event");
	}
}

}
