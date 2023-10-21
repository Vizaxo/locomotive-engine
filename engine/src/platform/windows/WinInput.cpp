#include "PCH.h"

#include "WinInput.h"

Keyboard::Key Platform::Windows::getKey(WPARAM wParam) {
	if (wParam >= 0x30 && wParam <= 0x39) {
		return static_cast<Keyboard::Key>(wParam - 0x30 + static_cast<WPARAM>(Keyboard::Key::NUM0));
	} else if (wParam >= 0x41 && wParam <= 0x5A) {
		return static_cast<Keyboard::Key>(wParam - 0x41 + static_cast<WPARAM>(Keyboard::Key::A));
	}

	return Keyboard::Key::Unknown;
}
