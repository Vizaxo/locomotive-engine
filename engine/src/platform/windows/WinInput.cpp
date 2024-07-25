#include "PCH.h"

// Windows-specific input code

#include "platform/Platform.h"
#ifdef PLATFORM_WINDOWS

#include "WinInput.h"

namespace PAL {
namespace Windows {

Keyboard::Key getKey(WPARAM wParam) {
	if (wParam >= 0x30 && wParam <= 0x39)
		return static_cast<Keyboard::Key>(wParam - 0x30 + static_cast<WPARAM>(Keyboard::Key::NUM0));
	if (wParam >= 0x41 && wParam <= 0x5A)
		return static_cast<Keyboard::Key>(wParam - 0x41 + static_cast<WPARAM>(Keyboard::Key::A));
	if (wParam >= VK_F1 && wParam <= VK_F24)
		return static_cast<Keyboard::Key>(wParam - VK_F1 + static_cast<WPARAM>(Keyboard::Key::F1));

	switch (wParam) {
	case VK_OEM_COMMA:		return Keyboard::Comma;
	case VK_OEM_PERIOD:		return Keyboard::Period;
	case VK_OEM_7:			return Keyboard::SingleQuote;
	case VK_LEFT:			return Keyboard::ArrowLeft;
	case VK_RIGHT:			return Keyboard::ArrowRight;
	case VK_UP:				return Keyboard::ArrowUp;
	case VK_DOWN:			return Keyboard::ArrowDown;
	default:				return Keyboard::Key::Unknown;
	}

}

/*
Mouse::Button getMouseButton(WPARAM wParam) {
	int b = Mouse::Button::NONE;
	if (wParam & MK_LBUTTON) b = b | Mouse::M1;
	if (wParam & MK_RBUTTON) b = b | Mouse::M2;
	if (wParam & MK_MBUTTON) b = b | Mouse::M3;
	if (wParam & MK_XBUTTON1) b = b | Mouse::M4;
	if (wParam & MK_XBUTTON2) b = b | Mouse::M5;
	return (Mouse::Button)b;
}
*/

}
}

#endif
