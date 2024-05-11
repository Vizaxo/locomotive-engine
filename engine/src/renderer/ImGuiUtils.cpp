#include "PCH.h"

#include "ImGuiUtils.h"

#ifdef PLATFORM_WINDOWS
#include "platform/windows/Windows.h"
#endif

void ImGui::SliderVectorF3(DirectX::XMVECTOR& v, float minBound, float maxBound, float floats[], const char* label) {
	ImGui::SliderFloat3(label, floats, minBound, maxBound, "%.3f");
	v = DirectX::XMVectorSet(floats[0], floats[1], floats[2], 0);
}

ImGuiWrapper::ImGuiWrapper(RefPtr<RHI> rhi, PAL::WindowHandle* h) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

#ifdef PLATFORM_WINDOWS
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(h->hwnd);
    ImGui_ImplDX11_Init(rhi->device.getRaw(), rhi->deviceContext.getRaw());
#else
#error "Platform not implemented for imgui"
#endif
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
