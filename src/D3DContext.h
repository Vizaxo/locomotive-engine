#pragma once
#include "DirectXTemplatePCH.h"

int InitDirectX(HINSTANCE hInstance, HWND windowHandle, bool vSync);
bool LoadContent(HWND windowHandle);
void Update(float deltaTime);
void Clear(const float clearColor[4], float clearDepth, uint8_t clearStencil);
void Present(bool vSync);
void Render(bool vSync);
void UnloadContent();
void Cleanup();
