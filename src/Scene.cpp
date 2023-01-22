#include "DirectXTemplatePCH.h"

#include "Scene.h"
#include "Object.h"
#include "D3DContext.h"

void Scene::RenderScene(D3DContext* d3dContext, float deltaTime) {
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

	d3dContext->Present();
}
