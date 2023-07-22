#pragma once

#include "DirectXTemplatePCH.h"

#include "renderer/D3DContext.h"
#include "imgui.h"

#define STR1(x)  #x
#define STR(x)  STR1(x)

#define MkSliderV3(label, v, minBound, maxBound) {\
	DirectX::XMVECTOR v2 = (v);\
	static float floats[3] = {DirectX::XMVectorGetX(v2), DirectX::XMVectorGetY(v2), DirectX::XMVectorGetZ(v2)}; \
	ImGui::SliderVectorF3((v), (minBound), (maxBound), floats, label "##" STR(__COUNTER__));\
}

namespace ImGui {
	void SliderVectorF3(DirectX::XMVECTOR& v, float minBound, float maxBound, float floats[], const char* label);
}

class ImGuiWrapper {
public:
	ImGuiWrapper(D3DContext* d3dContext, HWND hwnd);
	~ImGuiWrapper();
	void InitFrame();
};
