#include "PCH.h"

#include "ExampleProject.h"

#include "Utils.h"
#include "rhi/RHI.h"
#include "renderer/ImGuiUtils.h"
#include "renderer/Renderer.h"
#include "renderer/Material.h"
#include "renderer/Mesh.h"
#include "renderer/Texture.h"
#include "editor/ModelLoader.h"
#include "types/Pointers.h"
#include "Application.h"
#include "EngineMain.h"
#include "ecs/ECS.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "generated/BaseColourVertexShader.h"
#include "generated/BaseColourPixelShader.h"
#include "generated/TexturedPixelShader.h"
#include "generated/TexturedVertexShader.h"
#include "generated/FlatColorVS.h"
#include "generated/FlatColorPS.h"

enum MouseMode {
	Nop,
	MoveCamera,
};
MouseMode mouseMode = Nop;

Application* const application = new ExampleApplication();

PAL::WindowHandle* windowHandle = nullptr;

Log::Channel exampleGameChan = {"ExampleGame"};

void ExampleApplication::setupLighting() {
	scene.lightData.pointLights[scene.lightData.numPointLights].pos = DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f);
	scene.lightData.pointLights[scene.lightData.numPointLights].colour = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	scene.lightData.pointLights[scene.lightData.numPointLights].radius = 20.0f;
	scene.lightData.numPointLights++;

	XMStoreFloat3(&scene.lightData.directionalLight.direction, DirectX::XMVector3Normalize(DirectX::XMVectorSet(1.0f, -1.0f, 0.1f, 0.0f)));
	scene.lightData.directionalLight.colour = DirectX::XMFLOAT3(100.0f, 98.0f, 98.0f);
}

void ExampleApplication::setupCamera() {
	scene.camera.cam3d.pos = v3{0.0, 0.0, -1.0};
	scene.camera.cam3d.lookDirection = v3{0.0, 0.0, -1.0};
}

void ExampleApplication::init(RefPtr<Renderer> renderer, PAL::WindowHandle* h) {
	ECS::ecsManager.addComponentManager(new ECS::SparseSetComponentManager<SpriteComponent>());
	ECS::ecsManager.addComponentManager(new ECS::SparseSetComponentManager<StaticMeshComponent>());

	RefPtr<Material, true> solidColourMat = materialManager.get(sID("SolidColourMat"));
	RefPtr<Mesh> cubeMesh = Mesh::meshManager.get(sID("unitCube")).getNonNull();

	ECS::ecsManager.addComponent(ECS::generateEntity(), StaticMeshComponent({
		cubeMesh,
		solidColourMat.getNonNull(),
		{0.3f, 0.5f, 10.0f},
	}));

	auto res = ModelLoader::LoadModel(renderer->rhi, sID("DragonMesh"), L"resources/models/stanford_dragon_res3.ply");
	if (res.index() == 0) {
		RefPtr<Mesh> dragonMesh = std::get<RefPtr<Mesh, true>>(res).getNonNull();

		ECS::ecsManager.addComponent(ECS::generateEntity(), StaticMeshComponent({
			dragonMesh,
			solidColourMat.getNonNull(),
			{0.0f, -.5f, 1.0f},
			}));
	} else {
		LOG(Log::ERR, exampleGameChan, "Could not load dragon model. Error: %s", std::get<std::string>(res));
	}

	RefPtr<RHI::Texture2D> texture = Texture::loadTextureFromFile(renderer->rhi, "resources/textures/cube01.png").getNonNull();
	SpriteSheetCB cb = {{40, 40}, texture->size};
	static SpriteSheet spriteSheet = SpriteSheet{texture, cb};

	ECS::ecsManager.addComponent(ECS::generateEntity(), SpriteComponent::createSpriteComponent(renderer->rhi, {{100, 100}, {30, 40}}));
}

void ExampleApplication::tick(float dt) {
	if (Engine::vr) {
		scene.camera.cam3d.setCustomProj({Engine::vr->getProjectionMatrix(vr::Eye_Left, 0.1, 100.)});
		// TODO: set pos
	}

	static float t = 0;
	t += dt;
	for (auto it = ECS::ecsManager.view<StaticMeshComponent>(); !it->atEnd(); it->next()) {
		StaticMeshComponent& sp = **it;
		sp.pos.x = sin(t/2) / 2;
		sp.pos.y = cos(t/2) / 2;
	}
	/*
	{
		ImGui::Begin("Objects");
		if (scene.objects.size() >= 1)
			MkSliderV3("Plane", scene.objects[0].GetPos(), -10.0f, 10.0f);
		if (scene.objects.size() >= 2)
			MkSliderV3("Hex 1", scene.objects[1].GetPos(), -2.0f, 2.0f);
		if (scene.objects.size() >= 3)
			MkSliderV3("Textured cube", scene.objects[2].GetPos(), -2.0f, 2.0f);
		if (scene.objects.size() >= 4)
			MkSliderV3("Dragon", scene.objects[3].GetPos(), -5.0f, 5.0f);
		ImGui::End();
	}

	if (mouseMode == MoveCamera) {
		float mouseSensitivity = 0.001;
		float yaw = Mouse::dx * mouseSensitivity;
		float pitch = Mouse::dy * mouseSensitivity;
		scene.lookDirection = XMVector3Transform(scene.lookDirection, DirectX::XMMatrixRotationAxis(scene.worldUp, yaw));
		scene.lookDirection = XMVector3Transform(scene.lookDirection, DirectX::XMMatrixRotationAxis(scene.worldRight(), -pitch));
	}


	DirectX::XMVECTOR localDirection = XMVectorSet(0,0,0,0);
	if (Keyboard::keysDown.find(Keyboard::Key::W) != Keyboard::keysDown.end())
		localDirection += XMVectorSet(0,0,1,0);
	if (Keyboard::keysDown.find(Keyboard::Key::A) != Keyboard::keysDown.end())
		localDirection += XMVectorSet(1,0,0,0);
	if (Keyboard::keysDown.find(Keyboard::Key::S) != Keyboard::keysDown.end())
		localDirection += XMVectorSet(-1,0,0,0);
	if (Keyboard::keysDown.find(Keyboard::Key::D) != Keyboard::keysDown.end())
		localDirection += XMVectorSet(0,0,-1,0);

	DirectX::XMVECTOR worldDirection = DirectX::XMVector3Transform(localDirection, scene.localToWorld());

	const float movementSpeed = 5.0f;
	scene.eyePosition += movementSpeed * deltaTime * worldDirection;
	*/
}

RefPtr<Scene> ExampleApplication::getScene() {
	return &scene;
}

void ExampleApplication::mouseButtonDown(Mouse::Button b) {
	if (b == Mouse::Button::M2) {
		Mouse::lockCursorToWindow(windowHandle);
	}
	mouseMode = MoveCamera;
}

void ExampleApplication::mouseButtonUp(Mouse::Button b) {
	if (b == Mouse::Button::M2) {
		Mouse::unlockCursorFromWindow(windowHandle);
	}
	mouseMode = Nop;
}

void ExampleApplication::cleanup() {
}
