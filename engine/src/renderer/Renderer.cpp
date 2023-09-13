#include "Renderer.h"

#include "GBufferComposite.h"
#include "ScreenShader.h"
#include "ImGuiUtils.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "Utils.h"
#include "Light.h"
#include "Object.h"
#include "Mesh.h"
#include "Scene.h"

using namespace DirectX;

static std::vector<XMFLOAT3> triangleVertices = {
	XMFLOAT3(-1.0, -1.0, 0.0),
	XMFLOAT3(-1.0, 3.0, 0.0),
	XMFLOAT3(3.0, -1.0, 0.0),
};

Renderer::Renderer(D3DContext* d3dContext)
	: screenShader(d3dContext, g_ScreenShader, sizeof(g_ScreenShader))
	, GBufferCompositeShader(d3dContext, g_GBufferComposite, sizeof(g_GBufferComposite))
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutDesc = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11Device* d3dDevice = d3dContext->d3dDevice;
	GBufferCompositeInputLayout = screenShader.CreateInputLayout(d3dContext, vertexLayoutDesc);

	GBufferCompositeVertexBuffer = new VertexBuffer(toByteVector(triangleVertices), vertexLayoutDesc, sizeof(XMFLOAT3), 0);
	GBufferCompositeVertexBuffer->Initialise(d3dContext);

	// Create GBuffer
	D3D11_TEXTURE2D_DESC texture2DDesc = {
		d3dContext->clientWidth, d3dContext->clientHeight, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, {1, 0},
		D3D11_USAGE_DEFAULT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, 0
	};
	for (int i = 0; i < GBUFFER_COUNT; i++) {
		HRASSERT(d3dDevice->CreateTexture2D(&texture2DDesc, nullptr, &GBufferTextures[i]));
		HRASSERT(d3dDevice->CreateRenderTargetView(GBufferTextures[i], nullptr, &GBuffer[i]));
	}

	//Diffuse
	//SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	D3D11_TEX2D_SRV tex2DSRV = { 0, (UINT)-1};
	srvDesc.Texture2D = tex2DSRV;
	HRASSERT(d3dContext->d3dDevice->CreateShaderResourceView(GBufferTextures[DIFFUSE], &srvDesc, &GBufferDiffuseSRV));
	HRASSERT(d3dContext->d3dDevice->CreateShaderResourceView(GBufferTextures[NORMAL], &srvDesc, &GBufferNormalSRV));
	HRASSERT(d3dContext->d3dDevice->CreateShaderResourceView(GBufferTextures[WORLDPOS], &srvDesc, &GBufferWorldPosSRV));

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
	HRASSERT(d3dContext->d3dDevice->CreateSamplerState(&samplerDesc, &GBufferNormalSamplerState));
	HRASSERT(d3dContext->d3dDevice->CreateSamplerState(&samplerDesc, &GBufferWorldPosSamplerState));

	const D3D11_BUFFER_DESC cbFrameDataDesc = { sizeof(FrameData), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0 };
	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&cbFrameDataDesc, nullptr, &cbFrameData));

	const D3D11_BUFFER_DESC cbLightsDesc = { sizeof(LightData), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0 };
	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&cbLightsDesc, nullptr, &cbLights));

	// Constant buffers

	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HRASSERT(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &d3dConstantBuffers[CB_Application]));
	HRASSERT(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &d3dConstantBuffers[CB_Frame]));
	HRASSERT(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &d3dConstantBuffers[CB_Object]));

	ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), d3dContext->clientWidth / d3dContext->clientHeight, 0.1f, 100.0f);
	d3dContext->d3dDeviceContext->UpdateSubresource(d3dConstantBuffers[CB_Application], 0, nullptr, &ProjectionMatrix, 0, 0);

}

void Renderer::RenderScene(D3DContext* d3dContext, Scene& scene, float deltaTime) {
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	ViewMatrix = XMMatrixLookAtLH(scene.eyePosition, scene.focusPoint, upDirection);
	d3dContext->d3dDeviceContext->UpdateSubresource(d3dConstantBuffers[CB_Frame], 0, nullptr, &ViewMatrix, 0, 0);

	{
		ImGui::Begin("Stats");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	ID3D11DeviceContext* d3dDeviceContext = d3dContext->d3dDeviceContext;
	d3dContext->Clear(DirectX::Colors::CornflowerBlue, 1.0f, 0);

	d3dDeviceContext->VSSetConstantBuffers(0, 3, d3dConstantBuffers);

	d3dDeviceContext->RSSetState(d3dContext->d3dRasterizerState);
	d3dDeviceContext->RSSetViewports(1, &d3dContext->Viewport);

	d3dDeviceContext->OMSetRenderTargets(GBUFFER_COUNT, GBuffer, d3dContext->d3dDepthStencilView);
	d3dDeviceContext->OMSetDepthStencilState(d3dContext->d3dDepthStencilState, 1);

	d3dDeviceContext->ClearRenderTargetView(GBuffer[0], DirectX::Colors::Transparent);
	d3dDeviceContext->ClearRenderTargetView(GBuffer[1], DirectX::Colors::Transparent);
	d3dDeviceContext->ClearRenderTargetView(GBuffer[2], DirectX::Colors::Transparent);
	d3dDeviceContext->ClearDepthStencilView(d3dContext->d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	// Render to GBuffer
	for (Object& obj : scene.objects) {
		RenderObject(d3dContext, deltaTime, obj);
	}


	UINT32 stride = sizeof(XMFLOAT3);
	UINT32 offset = 0;
	GBufferCompositeVertexBuffer->Bind(d3dContext);
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	screenShader.Bind(d3dContext);
	GBufferCompositeShader.Bind(d3dContext);
	d3dDeviceContext->IASetInputLayout(GBufferCompositeInputLayout);

	d3dDeviceContext->OMSetRenderTargets(1, &d3dContext->BackBufferRTV, d3dContext->d3dDepthStencilView);

	//Set textures
	d3dDeviceContext->PSSetShaderResources(0, 1, &GBufferDiffuseSRV);
	d3dDeviceContext->PSSetSamplers(0, 1, &GBufferDiffuseSamplerState);

	d3dDeviceContext->PSSetShaderResources(1, 1, &GBufferNormalSRV);
	d3dDeviceContext->PSSetSamplers(1, 1, &GBufferNormalSamplerState);

	d3dDeviceContext->PSSetShaderResources(2, 1, &GBufferWorldPosSRV);
	d3dDeviceContext->PSSetSamplers(2, 1, &GBufferWorldPosSamplerState);

	FrameData frameData = {
		DirectX::XMFLOAT2(d3dContext->clientWidth, d3dContext->clientHeight),
		scene.eyePosition,
	};
	d3dDeviceContext->UpdateSubresource(cbFrameData, 0, nullptr, &frameData, 0, 0);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &cbFrameData);

	d3dDeviceContext->UpdateSubresource(cbLights, 0, nullptr, &scene.lightData, 0, 0);
	d3dDeviceContext->PSSetConstantBuffers(1, 1, &cbLights);

	d3dDeviceContext->Draw(3, 0);

	// Clear gbuffer textures so they can be rendered to
	ID3D11ShaderResourceView* nullSRVs[3] = {nullptr, nullptr, nullptr};
	d3dDeviceContext->PSSetShaderResources(0, 3, nullSRVs);

	// Re-set the render targets (without depth) so ImGui shows up as a separate pass in RenderDoc
	d3dDeviceContext->OMSetRenderTargets(1, &d3dContext->BackBufferRTV, nullptr);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	d3dContext->Present();
}

void Renderer::RenderObject(D3DContext* d3dContext, float deltaTime, Object& obj) {
	ID3D11DeviceContext* d3dDeviceContext = d3dContext->d3dDeviceContext;

	for (VertexBuffer& vertexBuffer : obj.mesh.meshData.vertexBuffers)
		vertexBuffer.Bind(d3dContext);

	d3dDeviceContext->IASetIndexBuffer(obj.mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	obj.mesh.vertexShader.Bind(d3dContext);
	obj.material->pixelShader.Bind(d3dContext);
	d3dDeviceContext->IASetInputLayout(obj.mesh.inputLayout);

	if (obj.material->shaderResourceView) {
		d3dDeviceContext->PSSetShaderResources(0, 1, &obj.material->shaderResourceView);
		d3dDeviceContext->PSSetSamplers(0, 1, &obj.material->samplerState);
	}

	DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMMATRIX modelMatrix = obj.GetModelMatrix();
	DirectX::XMMATRIX modelMatrixRotated = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(obj.angle)), modelMatrix);
	d3dDeviceContext->UpdateSubresource(d3dConstantBuffers[CB_Object], 0, nullptr, &modelMatrixRotated, 0, 0);

	d3dDeviceContext->DrawIndexed(obj.mesh.meshData.indices.size(), 0, 0);
}

Renderer::~Renderer() {
	SafeRelease(d3dConstantBuffers[CB_Application]);
	SafeRelease(d3dConstantBuffers[CB_Frame]);
	SafeRelease(d3dConstantBuffers[CB_Object]);

	for (int i = 0; i < GBUFFER_COUNT; i++)
		SafeRelease(GBuffer[i]);
}
