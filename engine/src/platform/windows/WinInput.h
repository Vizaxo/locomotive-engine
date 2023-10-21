#include  "PCH.h"

#pragma once

#include "input/Keyboard.h"

namespace Platform {
namespace Windows {

Keyboard::Key getKey(WPARAM wParam);

}
}
