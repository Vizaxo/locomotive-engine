#pragma once

#include "rhi/RHI.h"
#include "types/Pointers.h"
#include "Mesh.h"
#include "Material.h"
#include "Scene.h"
#include "StaticMeshComponent.h"
#include "platform/Platform.h"

struct Renderer {
	StaticMeshComponent createStaticMeshComponent(RefPtr<Mesh> mesh, RefPtr<Material> material, RefPtr<D3D11_INPUT_ELEMENT_DESC> descs, size_t count);
	void RenderScene(float deltaTime, RefPtr<Scene> scene);

	RHI rhi;
	RHI::RenderTargetView backBufferRTV;
	RHI::DepthStencilView backBufferDepthStencilView;
	OwningPtr<ID3D11DepthStencilState, false, ReleaseCOM> backBufferDepthStencilState;
	OwningPtr<ID3D11RasterizerState, false, ReleaseCOM> rasterizerState;
	D3D11_VIEWPORT fullScreenViewport;
};

OwningPtr<Renderer> createRenderer(RefPtr<PAL::WindowHandle> h);
