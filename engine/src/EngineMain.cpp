#include "PCH.h"
#include "EngineMain.h"

#include "debug/ttd.h"
#include "rhi/RHI.h"
#include "renderer/ImGuiUtils.h"
#include "renderer/Renderer.h"
#include "renderer2d/Renderer2D.h"
#include "Application.h"
#include "input/Mouse.h"
#include "core/Log.h"

namespace Engine {

EngineState engineState = EngineState::ENGINE_PRE_INIT;

ImGuiWrapper* imgui;
Renderer* renderer;

enum RenderMode {
	RENDER_3D,
	RENDER_2D,
};
static RenderMode renderMode = RENDER_3D;

D3DContext* d3dContext;
DWORD previousTime;

const float targetFramerate = 30.0f;
const float maxTimeStep = 1.0f / targetFramerate;

Log::Channel g_engineLog = {"engine"};

int init(PAL::WindowHandle* h, bool vSync) {
	LOG(Log::INFO, g_engineLog, "Initialising engine");
	engineState = ENGINE_INIT;
	d3dContext = new D3DContext(h, vSync);
	engineState = ENGINE_INIT_POST_D3D;

	if (d3dContext->d3dDevice == nullptr || d3dContext->d3dDeviceContext == nullptr) {
		MessageBox(nullptr, TEXT("Failed to create D3DContext"), TEXT("Error"), MB_OK);
		return -1;
	}

	imgui = new ImGuiWrapper(d3dContext, h);
	engineState = ENGINE_INIT_POST_IMGUI;

	switch (renderMode) {
	case RENDER_2D:
		Renderer2D::init(h->hwnd); //TODO: fix windows-specific code
		break;
	case RENDER_3D:
		renderer = new Renderer(d3dContext);
		break;
	}
	engineState = ENGINE_INIT_POST_RENDERER;
	previousTime = timeGetTime();

	engineState = ENGINE_POST_INIT;

	engineState = APP_PRE_INIT;
	application->init(d3dContext, h);
	engineState = APP_POST_INIT;
}

void tick() {
	engineState = ENGINE_TICKING;
	volatile bool debug = false;

	if (debug) {
		Debug::TTD::StartRecordTrace();
	}

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
		renderer->RenderScene(d3dContext, application->getScene(), deltaTime);
		break;
	}

	Mouse::endTick();

	if (debug) {
		Debug::TTD::StopRecordTrace();
	}
}

void cleanup() {
	engineState = ENGINE_CLEANUP;
	delete application;
	switch (renderMode) {
	case RENDER_2D:
		break;
	case RENDER_3D:
		delete renderer;
		break;
	}
	delete imgui;
	delete d3dContext;
	engineState = ENGINE_POST_CLEANUP;
}

void keyDown(Keyboard::Key k) {
	Keyboard::keyDown(k);
}

void keyUp(Keyboard::Key k) {
	Keyboard::keyUp(k);
}

}
