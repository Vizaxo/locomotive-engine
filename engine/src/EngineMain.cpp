#include "PCH.h"
#include "EngineMain.h"

#include "debug/ttd.h"
#include "debug/renderdoc/RenderDocModule.h"
#include "rhi/RHI.h"
#include "renderer/ImGuiUtils.h"
#include "renderer/Renderer.h"
#include "renderer2d/Renderer2D.h"
#include "Application.h"
#include "input/Mouse.h"
#include "input/Keyboard.h"
#include "core/Log.h"
#include "events/EventQueue.h"

namespace Engine {

EngineState engineState = EngineState::ENGINE_PRE_INIT;

ImGuiWrapper* imgui;
OwningPtr<Renderer, true> renderer = nullptr;
i32 g_frameNumber = -1;

enum RenderMode {
	RENDER_3D,
	RENDER_2D,
};
static RenderMode renderMode = RENDER_3D;

DWORD previousTime;

const float targetFramerate = 30.0f;
const float maxTimeStep = 1.0f / targetFramerate;

Log::Channel g_engineLog = {"engine"};
RefPtr<RenderDoc, true> renderdoc = nullptr;

int init(PAL::WindowHandle* h, bool vSync) {
	bool traceFromStart = false;
	if (traceFromStart) {
		LOG(Log::Level::INFO, g_engineLog, "Starting TTD trace");
		Debug::TTD::StartRecordTrace();
	}

	renderdoc = RenderDoc::loadRenderDoc();

	LOG(Log::INFO, g_engineLog, "Initialising engine");
	engineState = ENGINE_INIT;

	PAL::initialiseConsole();

	switch (renderMode) {
	case RENDER_2D:
		//Renderer2D::init(h->hwnd); //TODO: fix windows-specific code
		break;
	case RENDER_3D:
		renderer = createRenderer(h).getNullable();
		break;
	}
	engineState = ENGINE_INIT_POST_RENDERER;
	imgui = new ImGuiWrapper(renderer->rhi, h);
	engineState = ENGINE_INIT_POST_IMGUI;

	previousTime = timeGetTime();

	engineState = ENGINE_POST_INIT;

	engineState = APP_PRE_INIT;
	application->init(renderer.getRef().getNonNull(), h);
	engineState = APP_POST_INIT;

	return 1;
}

void tick() {
	g_frameNumber++;
	EventQueue::frameBeginEvent(g_frameNumber);

	while (std::optional<EventQueue::Event> ev = EventQueue::popEvent()) {
		switch (ev->type) {
		case EventQueue::EventType::Frame:
			// Ignore; we send these
			break;
		case EventQueue::EventType::Keyboard:
			Keyboard::handleKeyboardEvent(ev->keyboardEvent);
			break;
		case EventQueue::EventType::MouseButton:
			Mouse::handleMouseButtonEvent(ev->mouseButtonEvent);
			break;
		case EventQueue::EventType::MouseMove:
			Mouse::handleMouseMoveEvent(ev->mouseMoveEvent);
			break;
		}
	}

	engineState = ENGINE_TICKING;
	volatile bool debug = false;
	debug = debug || (Keyboard::keysDown.find(Keyboard::Key::F11) != Keyboard::keysDown.end());

	if (debug) {
		LOG(Log::Level::INFO, g_engineLog, "Starting TTD trace");
		Debug::TTD::StartRecordTrace();
	}

	if (renderdoc) renderdoc->tick();

	DWORD currentTime = timeGetTime();
	float deltaTime = (currentTime - previousTime) / 1000.0f;
	previousTime = currentTime;

	deltaTime = std::min<float>(deltaTime, maxTimeStep);

#if 0
	for (Keyboard::Key k : Keyboard::keysDown)
		DEBUG_PRINT(Keyboard::toString(k).c_str());
	if (!Keyboard::keysDown.empty())
		DEBUG_PRINT("\n");

	DEBUG_PRINT(std::to_string(Mouse::dx).c_str());
	DEBUG_PRINT(", ");
	DEBUG_PRINT(std::to_string(Mouse::dy).c_str());
	DEBUG_PRINT("\n");

	DEBUG_PRINT(std::to_string((int)Mouse::buttonState).c_str());
	DEBUG_PRINT("\n");
#endif

	imgui->InitFrame();
	application->tick(deltaTime);

	switch (renderMode) {
	case RENDER_2D:
		Renderer2D::renderScene(deltaTime);
		break;
	case RENDER_3D:
		renderer->RenderScene(deltaTime, application->getScene());
		break;
	}

	Mouse::endTick();

	if (Keyboard::keysDown.find(Keyboard::Key::F9) != Keyboard::keysDown.end()) {
		Debug::TTD::StopRecordTrace();
	}

	EventQueue::frameEndEvent(g_frameNumber);
}

void cleanup() {
	engineState = ENGINE_CLEANUP;
	delete application;
	delete imgui;
	engineState = ENGINE_POST_CLEANUP;
}

void keyDown(Keyboard::Key k) {
	Keyboard::keyDown(k);
}

void keyUp(Keyboard::Key k) {
	Keyboard::keyUp(k);
}

}
