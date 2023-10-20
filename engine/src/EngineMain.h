#pragma once

#include "platform/Platform.h"

namespace Engine {
	int init(PLATFORM_DATA platform);
	void tick();
	void cleanup();
};
