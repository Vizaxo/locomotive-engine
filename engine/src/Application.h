#pragma once

class Scene;
class D3DContext;

class Application {
public:
	virtual void init(D3DContext* d3dContext) = 0;
	virtual void tick(float deltaTime) = 0;
	virtual void cleanup() = 0;
	virtual Scene& getScene() = 0;
};

// To be defined by the game
extern Application* const application;
