#include "PCH.h"
#include "Renderer.h"

#include "Application.h"
#include "ConstantBuffers.h"
#include "EngineMain.h"
#include "Mesh.h"
#include "Texture.h"
#include "ecs/ECS.h"
#include "platform/windows/Windows.h"
#include "types/Matrix.h"
#include "ecs/components/TransformComponent.h"

v2f Renderer::getWindowSize() {
	Box2i clientRect = PAL::getClientRect(rhi->h);
	return v2f{(float)clientRect.width(), (float)clientRect.height()};
}

CB::ViewCB Renderer::makeViewCB(RefPtr<Camera::Camera> cam, RefPtr<TransformComponent> transform) {
	m44f model = transform->getMatrix();
	v2 windowSize = getWindowSize();
	m44 viewProj = cam->cam3d.viewProj(windowSize.x/windowSize.y, 0.1, 100.);
	m44 mvp = viewProj * model; //viewProj * model;

	return { windowSize, {0,0}, mvp, 0.f };
}

void Renderer::renderMesh(RefPtr<Mesh> mesh, RefPtr<TransformComponent> transform, RefPtr<Scene> scene, RefPtr<Material> material, size_t instances) {
	CB::ViewCB viewCB = makeViewCB(&scene->camera, transform);
	rhi->updateConstantBuffer<CB::ViewCB>(&material->constantBuffers[CB::View], viewCB); //TODO: ints being copied into floats. marshall or make them the same type

	for (int i = 0; i < RHI::CONSTANT_BUFFER_COUNT; ++i) {
		rhi->VSsetConstantBuffer(i, &material->constantBuffers[i]);
		rhi->PSsetConstantBuffer(i, &material->constantBuffers[i]);
	}
	rhi->setVertexBuffer(&mesh->vertexBuffer, 0);
	rhi->setIndexBuffer(&mesh->indexBuffer);
	rhi->setVertexShader(&material->vertexShader);
	rhi->setPixelShader(&material->pixelShader);
	RHI::InputLayout inputLayout = mesh->generateInputLayout(rhi, material);
	rhi->setInputLayout(&inputLayout);

	for (int i = 0; i < Material::MAX_TEXTURES; ++i) {
		if (RefPtr<RHI::Texture2D, true> t = material->textures[i]) {
			rhi->bindTextureSRV(i, t.getNonNull());
			rhi->bindSampler(i, t.getNonNull());
		}
	}

	//rhi->deviceContext->DrawIndexed(mesh->indexBuffer.indices, 0, 0);
	rhi->deviceContext->DrawIndexedInstanced(mesh->indexBuffer.count, instances, 0, 0, 0);
}

void Renderer::RenderScene(float deltaTime, RefPtr<Scene> scene) {
	backBufferRTV.clear(Colour::BLACK);
	backBufferDepthStencilView.clear(1.0f, 0u);

	rhi->deviceContext->RSSetState(rasterizerState.getRaw());
	rhi->deviceContext->RSSetViewports(1, &fullScreenViewport);

	rhi->deviceContext->OMSetRenderTargets(1, &backBufferRTV.rtv.getRaw(), backBufferDepthStencilView.depthStencilView.getRaw());
	rhi->deviceContext->OMSetDepthStencilState(backBufferDepthStencilState.getRaw(), 0);

	//for (int i = 0; i < scene->objects.num(); i++) {
	for (auto it = ECS::ecsManager.view<StaticMeshComponent>(); !it->atEnd(); it->next()) {
		StaticMeshComponent& meshComponent = **it;
		RefPtr<TransformComponent> transformComponent = ECS::ecsManager.getComponent<TransformComponent>(it->getIndex()).getNonNull(); //TODO: multi-component views
		renderMesh(meshComponent.mesh, transformComponent, scene, meshComponent.material, 1);
	}

	TransformComponent transform{};
	CB::ViewCB viewCB = makeViewCB(&scene->camera, &transform);
	{
		// Render sprites
		static RHI::BlendState alphaBlendState = rhi->createBlendState();

		// TODO: need to find a nice way of setting this in the game, and allowing for multiple atlases
		if (auto terrainTextureNullable = Texture::loadTextureFromFile(rhi, "resources/textures/terrain-isometric.png")) {
			// Dirty hack to only render sprites when application has right spritesheet.
			RefPtr<RHI::Texture2D> terrainTexture = terrainTextureNullable.getNonNull();
			static SpriteSheet terrainSpriteSheet = { terrainTexture, {{17, 21 }, terrainTexture->size } };

			rhi->setBlendState(&alphaBlendState);
			RefPtr<Mesh> unitSquare = Mesh::meshManager.get(sID("unitSquare")).getNonNull();
			RefPtr<Material> spriteMaterial = materialManager.get(sID("spriteMaterial")).getNonNull();

			ArrayView<ECS::Stored<SpriteComponent>> spriteData = ECS::ecsManager.getComponentManager<SpriteComponent>()->getRawBuffer();
			RHI::StructuredBuffer spriteBuffer = rhi->createStructuredBuffer<ECS::Stored<SpriteComponent>>(spriteData.data, spriteData.count);

			//TODO: lift these out of individual components
			RefPtr<SpriteSheet> spriteSheet = &terrainSpriteSheet;
			SpriteComponent& spriteComponent = spriteData.data.getRaw()[0].comp;

			rhi->updateConstantBuffer<CB::ViewCB>(&spriteMaterial->constantBuffers[CB::View], viewCB); //TODO: ints being copied into floats. marshall or make them the same type
			rhi->updateConstantBuffer(&spriteMaterial->constantBuffers[CB::Sprite], spriteComponent.cbData); //TODO: ints being copied into floats. marshall or make them the same type
			rhi->updateConstantBuffer(&spriteMaterial->constantBuffers[CB::SpriteSheet], spriteSheet->cbData);
			rhi->bindTextureSRV(0, spriteSheet->texture);
			rhi->bindStructuredBufferSRV(1, &spriteBuffer);
			rhi->bindSampler(0, spriteSheet->texture);
			renderMesh(unitSquare, {}, scene, spriteMaterial, spriteData.count);
		}
	}

	application->render(this, viewCB);
	if (Engine::vr) Engine::vr->render(this);

	rhi->deviceContext->OMSetRenderTargets(1, &backBufferRTV.rtv.getRaw(), nullptr);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	rhi->present();
}

OwningPtr<Renderer> createRenderer(RefPtr<PAL::WindowHandle> h) {
	OwningPtr<RHI> rhi = createRHI(h);
	RHI::RenderTargetView rtv = rhi->createBackBufferRTV();

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	ID3D11DepthStencilState* depthStencilState;
	rhi->device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	Box2i clientRect = PAL::getClientRect(h);
	static const DXGI_FORMAT DEPTH_STENCIL_FORMAT = DXGI_FORMAT_D24_UNORM_S8_UINT;
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {};
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
    depthStencilBufferDesc.Format = DEPTH_STENCIL_FORMAT;
    depthStencilBufferDesc.Width = clientRect.width();
    depthStencilBufferDesc.Height = clientRect.height();
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.SampleDesc.Count = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D* depthStencilBuffer;
	HRASSERT(rhi->device->CreateTexture2D( &depthStencilBufferDesc, nullptr, &depthStencilBuffer ));
	ID3D11DepthStencilView* depthStencilView;
	HRASSERT(rhi->device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView));
	RHI::DepthStencilView backBufferDepthStencilView = {std::move(depthStencilView), DEPTH_STENCIL_FORMAT, rhi.getRef()};

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state object.
	ID3D11RasterizerState* rasterizerState;
	HRASSERT(rhi->device->CreateRasterizerState(&rasterizerDesc, &rasterizerState));

	D3D11_VIEWPORT fullScreenViewport = {};
	fullScreenViewport.TopLeftX = clientRect.min.x;
	fullScreenViewport.TopLeftY = clientRect.min.y;
	fullScreenViewport.Width = clientRect.width();
	fullScreenViewport.Height = clientRect.height();
	fullScreenViewport.MinDepth = 0.0f;
	fullScreenViewport.MaxDepth = 1.0f;

	Mesh::registerSimpleMeshes(rhi.getRef());
	Material::createBasicMaterials(rhi.getRef());

	v2f windowSize = v2f{(float)clientRect.width(), (float)clientRect.height()};
	RefPtr<Material, true> spriteMaterial = createSpriteMaterial(rhi, windowSize);
	return new Renderer({std::move(rhi), std::move(rtv), std::move(backBufferDepthStencilView), std::move(depthStencilState), std::move(rasterizerState), fullScreenViewport, spriteMaterial.getNonNull()});
}
