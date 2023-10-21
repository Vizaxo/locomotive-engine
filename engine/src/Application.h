#include "PCH.h"

#pragma once

#include "input/Mouse.h"

class Scene;
class D3DContext;

namespace Application {

void init(D3DContext* d3dContext, PAL::WindowHandle* h);
void tick(float deltaTime);
void cleanup();
Scene& getScene();
void mouseButtonDown(Mouse::Button b);
void mouseButtonUp(Mouse::Button b);

};
