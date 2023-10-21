#pragma once

#include "platform/Platform.h"
#include "input/Keyboard.h"

namespace Engine {
	int init(PLATFORM_DATA platform);
	void tick();
	void cleanup();
	void keyDown(Keyboard::Key k);
	void keyUp(Keyboard::Key k);
};
