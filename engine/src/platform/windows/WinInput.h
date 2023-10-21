#include  "PCH.h"

#pragma once

#include "platform/windows/Windows.h"
WINDOWS_ONLY

#include "input/Keyboard.h"
#include "input/Mouse.h"

namespace PAL {
namespace Windows {

Keyboard::Key getKey(WPARAM wParam);

Mouse::Button getMouseButton(WPARAM wParam);

}
}
