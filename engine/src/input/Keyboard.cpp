#include "Keyboard.h"

Keyboard::Key Keyboard::getKey(WPARAM wParam) {
	if (wParam >= 0x30 && wParam <= 0x39) {
		return static_cast<Key>(wParam - 0x30 + static_cast<WPARAM>(Key::NUM0));
	} else if (wParam >= 0x41 && wParam <= 0x5A) {
		return static_cast<Key>(wParam - 0x41 + static_cast<WPARAM>(Key::A));
	}

	return Key::Unknown;
}
