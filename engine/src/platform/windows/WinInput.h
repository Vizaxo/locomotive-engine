#include  "PCH.h"

#pragma once

#include "input/Keyboard.h"
#include "input/Mouse.h"

namespace Platform {
namespace Windows {

Keyboard::Key getKey(WPARAM wParam);

Mouse::Button getMouseButton(WPARAM wParam);

}
}
