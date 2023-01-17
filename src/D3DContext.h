#pragma once

#include "DirectXTemplatePCH.h"


class D3DContext {
public:
	D3DContext(HINSTANCE hInstance, HWND windowHandle, bool vSync);
	~D3DContext();
	void Update(float deltaTime);
	void Clear(const float clearColor[4], float clearDepth, uint8_t clearStencil);
	void Present();
	void Render();
};

