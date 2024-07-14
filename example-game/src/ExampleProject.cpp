#include "PCH.h"

#include "ExampleProject.h"

#include "Utils.h"
#include "rhi/RHI.h"
#include "renderer/ImGuiUtils.h"
#include "renderer/Renderer.h"
#include "renderer/Material.h"
#include "editor/ModelLoader.h"
#include "renderer/Mesh.h"
#include "types/HexCoord.h"
#include "types/Pointers.h"
#include "world/World.h"
#include "Application.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "generated/BaseColourVertexShader.h"
#include "generated/BaseColourPixelShader.h"
#include "generated/TexturedPixelShader.h"
#include "generated/TexturedVertexShader.h"
#include "generated/FlatColorVS.h"
#include "generated/FlatColorPS.h"

using namespace DirectX;

std::vector<VertexPosColor> triangleVertices = {
	{XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
};

std::vector<DirectX::XMFLOAT3> planePositions = {
	{XMFLOAT3(-100.0f, -2.0f, -100.0f)},
	{XMFLOAT3(-100.0f, -2.0f, 100.0f)},
	{XMFLOAT3(100.0f, -2.0f, -100.0f)},
	{XMFLOAT3(100.0f, -2.0f, 100.0f)},
};

std::vector<int> planeIndices = {
	{0, 1, 2, 1, 3, 2}
};

const float sqrt3 = sqrt(3);
std::vector<DirectX::XMFLOAT3> hexPrismPositions = {
	{XMFLOAT3(0.f,       0.f,    0.f       )},
	{XMFLOAT3(1.f/2.f,   0.f,    sqrt3/2.f )},
	{XMFLOAT3(1.f,       0.f,    0.f       )},
	{XMFLOAT3(1.f/2.f,   0.f,    -sqrt3/2.f)},
	{XMFLOAT3(-1.f/2.f,  0.f,    -sqrt3/2.f)},
	{XMFLOAT3(-1.f,      0.f,    0.f       )},
	{XMFLOAT3(-1.f/2.f,  0.f,    sqrt3/2.f )},
	{XMFLOAT3(0.f,       -100.f, 0.f       )},
	{XMFLOAT3(1.f/2.f,   -100.f, sqrt3/2.f )},
	{XMFLOAT3(1.f,       -100.f, 0.f       )},
	{XMFLOAT3(1.f/2.f,   -100.f, -sqrt3/2.f)},
	{XMFLOAT3(-1.f/2.f,  -100.f, -sqrt3/2.f)},
	{XMFLOAT3(-1.f,      -100.f, 0.f       )},
	{XMFLOAT3(-1.f/2.f,  -100.f, sqrt3/2.f )},
};

std::vector<int> hexPrismIndices = {
	//top
	0,1,2,
	0,2,3,
	0,3,4,
	0,4,5,
	0,5,6,
	0,6,1,
	//sides
	2,1,8,
	2,8,9,
	3,2,9,
	3,9,10,
	4,3,10,
	4,10,11,
	5,4,11,
	5,11,12,
	6,5,12,
	6,12,13,
	1,6,13,
	1,13,8,
	//bottom
	9,8,7,
	10,9,7,
	11,10,7,
	12,11,7,
	13,12,7,
	8,13,7,
};

enum MouseMode {
	Nop,
	MoveCamera,
};
MouseMode mouseMode = Nop;

std::vector<int> triangleIndices = { 0,1,2,5,4,3 };

Application* const application = new ExampleApplication();

PAL::WindowHandle* windowHandle = nullptr;

/*
void ExampleApplication::makePlane(RefPtr<RHI> rhi) {
	std::vector<DirectX::XMFLOAT3> planeColours, planeNormals;
	for (int i = 0; i < planePositions.size(); i++) {
		planeNormals.push_back(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
		planeColours.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	}
	VertexBuffer planePositionsVB = CreateVertexBuffer(planePositions,
		{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		0);
	VertexBuffer planeColoursVB = CreateVertexBuffer(planeColours,
		{ {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		1);
	VertexBuffer planeNormalsVB = CreateVertexBuffer(planeNormals,
		{ {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		2);

	OwningPtr<MeshData> planeMeshData = new MeshData({ planePositionsVB, planeColoursVB, planeNormalsVB }, std::move(planeIndices));
	OwningPtr<Mesh> planeMesh = new Mesh(d3dContext, std::move(planeMeshData), *baseColourVertexShader);

	RefPtr<Mesh> planeMeshRef = meshManager.registerResource(internStringId("plane_mesh"), std::move(planeMesh)).getNonNull();
	RefPtr<Material, true> baseColourMaterial = materialManager.get(internStringId("base_colour_material"));
	if (baseColourMaterial) {
		Object plane = Object(d3dContext, XMVectorSet(-2, 0, 0, 0), 0.0f, planeMeshRef, baseColourMaterial.getNonNull());
		scene.objects.push_back(plane);
	} else {
		assert(false, "Could not find base_colour_material");
	}
}
*/

	/*
RefPtr<Mesh> ExampleApplication::createHexMesh(D3DContext* d3dContext) {
	VertexBuffer hexPrismPosVB = CreateVertexBuffer(hexPrismPositions,
		{  {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}},
		0);
	std::vector<DirectX::XMFLOAT3> hexPrismNormals = std::vector<DirectX::XMFLOAT3>(hexPrismPositions.size());
	for (size_t i = 0; i < hexPrismIndices.size(); i += 3) {
		DirectX::XMFLOAT3* v1 = (DirectX::XMFLOAT3*)&hexPrismPositions[hexPrismIndices[i]];
		DirectX::XMFLOAT3* v2 = (DirectX::XMFLOAT3*)&hexPrismPositions[hexPrismIndices[i + 1]];
		DirectX::XMFLOAT3* v3 = (DirectX::XMFLOAT3*)&hexPrismPositions[hexPrismIndices[i + 2]];
		DirectX::XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(XMLoadFloat3(v2), XMLoadFloat3(v1))
			, DirectX::XMVectorSubtract(XMLoadFloat3(v3), XMLoadFloat3(v1))));
		DirectX::XMFLOAT3 faceNormalV3;
		XMStoreFloat3(&faceNormalV3, faceNormal);

		hexPrismNormals[hexPrismIndices[i]] = faceNormalV3;
		hexPrismNormals[hexPrismIndices[i+1]] = faceNormalV3;
		hexPrismNormals[hexPrismIndices[i+2]] = faceNormalV3;
	}


	std::vector<DirectX::XMFLOAT3> hexPrismColours = std::vector<DirectX::XMFLOAT3>(hexPrismPositions.size());
	for (size_t i = 0; i < hexPrismPositions.size(); i++) {
		hexPrismColours[i] = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	}
	VertexBuffer hexPrismColoursVB = CreateVertexBuffer(hexPrismColours, 
		{ {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}},
		1);

	VertexBuffer hexPrismNormalsVB = CreateVertexBuffer(hexPrismNormals,
		{ {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		2);
	OwningPtr<MeshData> hexMeshData = new MeshData({hexPrismPosVB, hexPrismColoursVB, hexPrismNormalsVB}, std::move(hexPrismIndices));
	OwningPtr<Mesh> hexMesh = new Mesh(d3dContext, std::move(hexMeshData), *baseColourVertexShader);

	return meshManager.registerResource(internStringId("hex_mesh"), std::move(hexMesh)).getNonNull();
}
*/

void ExampleApplication::setupLighting() {
	scene.lightData.pointLights[scene.lightData.numPointLights].pos = DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f);
	scene.lightData.pointLights[scene.lightData.numPointLights].colour = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	scene.lightData.pointLights[scene.lightData.numPointLights].radius = 20.0f;
	scene.lightData.numPointLights++;

	XMStoreFloat3(&scene.lightData.directionalLight.direction, DirectX::XMVector3Normalize(DirectX::XMVectorSet(1.0f, -1.0f, 0.1f, 0.0f)));
	scene.lightData.directionalLight.colour = DirectX::XMFLOAT3(100.0f, 98.0f, 98.0f);
}

void ExampleApplication::setupCamera() {
	scene.eyePosition = XMVectorSet(0, 10, -10, 1);
	scene.lookDirection = XMVectorSet(0, -0.2, 1, 0);
}

void ExampleApplication::init(RefPtr<Renderer> renderer, PAL::WindowHandle* h) {
	windowHandle = h;

	RHI::VertexShader vs = renderer->rhi->createVertexShaderFromBytecode((u8*)flatColorVSBytecode, sizeof(flatColorVSBytecode));
	RHI::PixelShader ps = renderer->rhi->createPixelShaderFromBytecode((u8*)flatColorPSBytecode, sizeof(flatColorPSBytecode));

	D3D11_INPUT_ELEMENT_DESC inputLayoutDescs[1];
	inputLayoutDescs[0].SemanticName = "POSITION";
	inputLayoutDescs[0].SemanticIndex = 0;
	inputLayoutDescs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayoutDescs[0].InputSlot = 0;
	inputLayoutDescs[0].AlignedByteOffset = 0;
	inputLayoutDescs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDescs[0].InstanceDataStepRate = 0;
	RHI::InputLayout inputLayout = renderer->rhi->createInputLayout(inputLayoutDescs, 1, &vs);
	OwningPtr<Material> flatColorMat = new Material{
		std::move(vs),
		std::move(ps)
	};
	v3f color = { 1.0f, 0.5f, 0.2f };
	flatColorMat->constantBuffers[0] = renderer->rhi->createConstantBuffer(color);
	RefPtr<Material, true> flatColorMatRegistered = materialManager.registerResource(internStringId("flatColorMat"), std::move(flatColorMat));
	RefPtr<Mesh> cubeMesh = Mesh::meshManager.get(sID("unitCube")).getNonNull();

	// Use placement new to create object in the array
	scene.objects = (StaticMeshComponent*)operator new[](1 * sizeof (StaticMeshComponent));
	scene.obj_count = 1;
	new (&scene.objects.getRaw()[0]) StaticMeshComponent({
		cubeMesh,
		flatColorMatRegistered.getNonNull(),
		std::move(inputLayout),
		{0.0f, 0.0f, 0.0f},
	});

	static const u32 SPRITE_INPUT_DESC_COUNT = 2;
	D3D11_INPUT_ELEMENT_DESC spriteInputLayoutDescs[SPRITE_INPUT_DESC_COUNT];
	spriteInputLayoutDescs[0].SemanticName = "POSITION";
	spriteInputLayoutDescs[0].SemanticIndex = 0;
	spriteInputLayoutDescs[0].Format = DXGI_FORMAT_R32G32_FLOAT;
	spriteInputLayoutDescs[0].InputSlot = 0;
	spriteInputLayoutDescs[0].AlignedByteOffset = 0;
	spriteInputLayoutDescs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	spriteInputLayoutDescs[0].InstanceDataStepRate = 0;
	spriteInputLayoutDescs[1].SemanticName = "TEXCOORD";
	spriteInputLayoutDescs[1].SemanticIndex = 0;
	spriteInputLayoutDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	spriteInputLayoutDescs[1].InputSlot = 0;
	spriteInputLayoutDescs[1].AlignedByteOffset = sizeof(v2f);
	spriteInputLayoutDescs[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	spriteInputLayoutDescs[1].InstanceDataStepRate = 0;
	RHI::InputLayout spriteInputLayout = renderer->rhi->createInputLayout(spriteInputLayoutDescs, SPRITE_INPUT_DESC_COUNT, &materialManager.get(sID("spriteMaterial"))->vertexShader);
	scene.sprite_count = 1;
	scene.sprites = (SpriteComponent*)operator new[](scene.sprite_count * sizeof(SpriteComponent));
	new (&scene.sprites.getRaw()[0]) SpriteComponent({
		{100, 100},
		{30, 40},
		{0.0f, 1.0f, 0.0f},
		std::move(spriteInputLayout),
	});


	//scene = new StaticMeshComponent[1]();

	//baseColourVertexShader = new VertexShader(d3dContext, (const void*)g_BaseColourVertexShader, sizeof(g_BaseColourVertexShader));
	//baseColourPixelShader = new PixelShader(d3dContext, g_ps, sizeof(g_ps));
	//RefPtr<Material> baseColourMaterial = materialManager.registerResource(internStringId("base_colour_material"), new Material(*baseColourPixelShader)).getNonNull();

	//makePlane(d3dContext);
	//RefPtr<Mesh> hexMesh = createHexMesh(d3dContext);

	/*
	int width = 100;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			HexCoord c = {i-width/2, j-width/2};
			scene.objects.push_back(Object(d3dContext, XMVectorSet(c.cartesian().x, World::getHeight(c), c.cartesian().y, 0.0f), 90.0f, hexMesh, baseColourMaterial));
		}
	}

	setupLighting();
	setupCamera();
	*/
}

void ExampleApplication::tick(float deltaTime) {
	scene.sprites.getRaw()[0].size.y += 1;
	scene.sprites.getRaw()[0].pos.x += 1;
	scene.sprites.getRaw()[0].pos.x = fmod(scene.sprites.getRaw()[0].pos.x, 1000);
	/*
	{
		ImGui::Begin("Objects");                          // Create a window called "Hello, world!" and append into it.
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
