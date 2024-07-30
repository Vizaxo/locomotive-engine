#pragma once

#include "platform/Platform.h"
#include "input/Keyboard.h"
#include "debug/renderdoc/RenderDocModule.h"
#include "vr/OpenVRModule.h"

namespace Engine {

int init(PAL::WindowHandle* h, bool vsync);
void tick();
void cleanup();
void keyDown(Keyboard::Key k);
void keyUp(Keyboard::Key k);

enum EngineState {
	ENGINE_PRE_INIT,
	ENGINE_INIT,
	ENGINE_INIT_POST_IMGUI,
	ENGINE_INIT_POST_RENDERER,
	ENGINE_POST_INIT,
	APP_PRE_INIT,
	APP_POST_INIT,
	ENGINE_TICKING,
	ENGINE_CLEANUP,
	ENGINE_POST_CLEANUP,

	NUM_ENGINE_STATES
};

extern EngineState engineState;
extern RefPtr<RenderDoc, true> renderdoc;
extern OwningPtr<VR::VRModule, true> vr;

};

