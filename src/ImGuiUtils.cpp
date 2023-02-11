#include "DirectXTemplatePCH.h"

#include "ImGuiUtils.h"

void ImGui::SliderVectorF3(DirectX::XMVECTOR& v, float minBound, float maxBound, float floats[], const char* label) {
	ImGui::SliderFloat3(label, floats, minBound, maxBound, "%.3f");
	v = DirectX::XMVectorSet(floats[0], floats[1], floats[2], 0);
}
