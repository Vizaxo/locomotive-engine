#include "DirectXTemplatePCH.h"

#include "Scene.h"
#include "Object.h"
#include "D3DContext.h"
#include "ImGuiUtils.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "GBufferComposite.h"
#include "ScreenShader.h"

using namespace DirectX;

XMFLOAT3 triangleVertices[3] = {
	XMFLOAT3(-1.0, -1.0, 0.0),
	XMFLOAT3(0.0, 1.0, 0.0),
	XMFLOAT3(1.0, -1.0, 0.0),
};

void Scene::Initialise(D3DContext* d3dContext) {
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11Device* d3dDevice = d3dContext->d3dDevice;
	HRASSERT(d3dDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), g_ScreenShader, sizeof(g_ScreenShader), &GBufferCompositeInputLayout));
	HRASSERT(d3dDevice->CreateVertexShader(g_ScreenShader, sizeof(g_ScreenShader), nullptr, &ScreenShader));
	HRASSERT(d3dDevice->CreatePixelShader(g_GBufferComposite, sizeof(g_GBufferComposite), nullptr, &GBufferCompositeShader));



	// Setup vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * 9;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = triangleVertices;

	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &GBufferCompositeVertexBuffer));


	//Diffuse
	//SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	D3D11_TEX2D_SRV tex2DSRV = { 0, (UINT)-1};
	srvDesc.Texture2D = tex2DSRV;
	HRASSERT(d3dContext->d3dDevice->CreateShaderResourceView(d3dContext->GBufferTextures[DIFFUSE], &srvDesc, &GBufferDiffuseSRV));

	//Sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRASSERT(d3dContext->d3dDevice->CreateSamplerState(&samplerDesc, &GBufferDiffuseSamplerState));
}

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

	d3dDeviceContext->OMSetRenderTargets(GBUFFER_COUNT, d3dContext->GBuffer, d3dContext->d3dDepthStencilView);
	d3dDeviceContext->OMSetDepthStencilState(d3dContext->d3dDepthStencilState, 1);

	d3dDeviceContext->ClearRenderTargetView(d3dContext->GBuffer[0], DirectX::Colors::CornflowerBlue);
	d3dDeviceContext->ClearDepthStencilView(d3dContext->d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	// Render to GBuffer
	for (IObject* obj : objects) {
		obj->RenderObject(d3dContext, deltaTime);
	}


	UINT32 stride = sizeof(XMFLOAT3);
	UINT32 offset = 0;
	d3dDeviceContext->IASetVertexBuffers(0, 1, &GBufferCompositeVertexBuffer, &stride, &offset);
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dDeviceContext->VSSetShader(ScreenShader, nullptr, 0);
	d3dDeviceContext->PSSetShader(GBufferCompositeShader, nullptr, 0);
	d3dDeviceContext->IASetInputLayout(GBufferCompositeInputLayout);

	d3dDeviceContext->OMSetRenderTargets(1, &d3dContext->BackBufferRTV, d3dContext->d3dDepthStencilView);

	//Set textures
	d3dDeviceContext->PSSetShaderResources(0, 1, &GBufferDiffuseSRV);
	d3dDeviceContext->PSSetSamplers(0, 1, &GBufferDiffuseSamplerState);


	d3dDeviceContext->Draw(3, 0);

	// Clear gbuffer textures so they can be rendered to
	d3dDeviceContext->PSSetShaderResources(0, 0, nullptr);
	d3dDeviceContext->PSSetSamplers(0, 0, nullptr);

	// Re-set the render targets (without depth) so ImGui shows up as a separate pass in RenderDoc
	d3dDeviceContext->OMSetRenderTargets(1, &d3dContext->BackBufferRTV, nullptr);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	d3dContext->Present();
}
