#include "DirectXTemplatePCH.h"

#include "ImGuiUtils.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void ImGui::SliderVectorF3(DirectX::XMVECTOR& v, float minBound, float maxBound, float floats[], const char* label) {
	ImGui::SliderFloat3(label, floats, minBound, maxBound, "%.3f");
	v = DirectX::XMVectorSet(floats[0], floats[1], floats[2], 0);
}

ImGuiWrapper::ImGuiWrapper(D3DContext* d3dContext, HWND hwnd) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(d3dContext->d3dDevice, d3dContext->d3dDeviceContext);
}

ImGuiWrapper::~ImGuiWrapper() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiWrapper::InitFrame() {
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
