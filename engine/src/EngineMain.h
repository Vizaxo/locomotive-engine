#pragma once

#include "platform/Platform.h"
#include "input/Keyboard.h"

namespace Engine {

int init(PAL::WindowHandle* h, bool vsync);
void tick();
void cleanup();
void keyDown(Keyboard::Key k);
void keyUp(Keyboard::Key k);

enum EngineState {
	ENGINE_PRE_INIT,
	ENGINE_INIT,
	ENGINE_INIT_POST_D3D,
	ENGINE_INIT_POST_IMGUI,
	ENGINE_INIT_POST_RENDERER,
	ENGINE_POST_INIT,
	APP_PRE_INIT,
	APP_POST_INIT,
	ENGINE_TICKING,
	ENGINE_CLEANUP,
	ENGINE_POST_CLEANUP,
};

extern EngineState engineState;

};

