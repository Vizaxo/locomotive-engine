#include "PCH.h"

#pragma once

#include "input/Mouse.h"

class Scene;
class D3DContext;

class Application {
public:
	virtual void init(D3DContext* d3dContext, PAL::WindowHandle* h) = 0;
	virtual void tick(float deltaTime) = 0;
	virtual void cleanup() = 0;
	virtual Scene& getScene() = 0;
	virtual void mouseButtonDown(Mouse::Button b) = 0;
	virtual void mouseButtonUp(Mouse::Button b) = 0;
};

// To be defined by the game
extern Application* const application;
