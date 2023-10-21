#include "PCH.h"
#include "EngineMain.h"

#include "debug/ttd.h"
#include "rhi/RHI.h"
#include "renderer/ImGuiUtils.h"
#include "renderer/Renderer.h"
#include "Application.h"
#include "input/Mouse.h"

namespace Engine {

ImGuiWrapper* imgui;
Renderer* renderer;
D3DContext* d3dContext;
DWORD previousTime;

const float targetFramerate = 30.0f;
const float maxTimeStep = 1.0f / targetFramerate;

int init(PAL::WindowHandle* h, bool vSync) {
	d3dContext = new D3DContext(h, vSync);
	if (d3dContext->d3dDevice == nullptr || d3dContext->d3dDeviceContext == nullptr) {
		MessageBox(nullptr, TEXT("Failed to create D3DContext"), TEXT("Error"), MB_OK);
		return -1;
	}

	imgui = new ImGuiWrapper(d3dContext, h);
	renderer = new Renderer(d3dContext);
	previousTime = timeGetTime();

	Application::init(d3dContext, h);
}

void tick() {
	volatile bool debug = false;

	if (debug) {
		Debug::TTD::StartRecordTrace();
	}

	DWORD currentTime = timeGetTime();
	float deltaTime = (currentTime - previousTime) / 1000.0f;
	previousTime = currentTime;

	deltaTime = std::min<float>(deltaTime, maxTimeStep);

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

	imgui->InitFrame();
	Application::tick(deltaTime);
	renderer->RenderScene(d3dContext, Application::getScene(), deltaTime);
	Mouse::endTick();

	if (debug) {
		Debug::TTD::StopRecordTrace();
	}
}

void cleanup() {
	delete renderer;
	delete imgui;
	delete d3dContext;
}

void keyDown(Keyboard::Key k) {
	Keyboard::keyDown(k);
}

void keyUp(Keyboard::Key k) {
	Keyboard::keyUp(k);
}

}
