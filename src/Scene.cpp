#include "DirectXTemplatePCH.h"

#include "Scene.h"
#include "Object.h"
#include "D3DContext.h"
#include "ImGuiUtils.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void Scene::RenderScene(D3DContext* d3dContext, float deltaTime) {
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("Objects");                          // Create a window called "Hello, world!" and append into it.
		MkSliderV3("Coloured cube", objects[0]->GetPos(), -2.0f, 2.0f);
		MkSliderV3("Coloured triangle", objects[1]->GetPos(), -2.0f, 2.0f);
		MkSliderV3("Textured cube", objects[2]->GetPos(), -2.0f, 2.0f);
		ImGui::End();
	}

	{
		ImGui::Begin("Stats");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	ID3D11DeviceContext* d3dDeviceContext = d3dContext->d3dDeviceContext;
	d3dContext->Clear(DirectX::Colors::CornflowerBlue, 1.0f, 0);

	d3dDeviceContext->VSSetConstantBuffers(0, 3, d3dContext->d3dConstantBuffers);

	d3dDeviceContext->RSSetState(d3dContext->d3dRasterizerState);
	d3dDeviceContext->RSSetViewports(1, &d3dContext->Viewport);

	d3dDeviceContext->OMSetRenderTargets(1, &d3dContext->d3dRenderTargetView, d3dContext->d3dDepthStencilView);
	d3dDeviceContext->OMSetDepthStencilState(d3dContext->d3dDepthStencilState, 1);

	for (IObject* obj : objects) {
		obj->RenderObject(d3dContext, deltaTime);
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	d3dContext->Present();
}
