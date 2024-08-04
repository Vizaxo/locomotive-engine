#pragma once

#include "ConstantBuffers.h"
#include "rhi/RHI.h"
#include "types/Pointers.h"
#include "Mesh.h"
#include "Material.h"
#include "Scene.h"
#include "StaticMeshComponent.h"
#include "ecs/components/TransformComponent.h"
#include "platform/Platform.h"

struct Renderer {
	StaticMeshComponent createStaticMeshComponent(RefPtr<Mesh> mesh, RefPtr<Material> material, size_t count);
	void RenderScene(float deltaTime, RefPtr<Scene> scene);
	v2f getWindowSize();
	void renderMesh(RefPtr<Mesh> mesh, RefPtr<TransformComponent> transform, RefPtr<Scene> scene, RefPtr<Material> material, size_t instances);
	CB::ViewCB makeViewCB(RefPtr<Camera::Camera> cam, RefPtr<TransformComponent> transform);

	OwningPtr<RHI> rhi;
	RHI::RenderTargetView backBufferRTV;
	RHI::DepthStencilView backBufferDepthStencilView;
	OwningPtr<ID3D11DepthStencilState, false, ReleaseCOM> backBufferDepthStencilState;
	OwningPtr<ID3D11RasterizerState, false, ReleaseCOM> rasterizerState;
	D3D11_VIEWPORT fullScreenViewport;
	RefPtr<Material> spriteMaterial;
};

OwningPtr<Renderer> createRenderer(RefPtr<PAL::WindowHandle> h);
