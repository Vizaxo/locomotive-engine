#include "ExampleScene.h"

#include "Utils.h"
#include "rhi/RHI.h"
#include "renderer/ImGuiUtils.h"
#include "renderer/Object.h"
#include "renderer/Material.h"
#include "editor/ModelLoader.h"
#include "renderer/Mesh.h"
#include "HexCoord.h"
#include "world/World.h"

#include "BaseColourVertexShader.h"
#include "BaseColourPixelShader.h"
#include "TexturedPixelShader.h"
#include "TexturedVertexShader.h"

using namespace DirectX;

std::vector<DirectX::XMFLOAT3> cubePositions = {
	{XMFLOAT3(-1.0f, -1.0f, -1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f)},
	{XMFLOAT3(1.0f,  1.0f, -1.0f)},
	{XMFLOAT3(1.0f,  -1.0f, -1.0f)},
	{XMFLOAT3(-1.0f, -1.0f,  1.0f)},
	{XMFLOAT3(-1.0f, 1.0f,  1.0f)},
	{XMFLOAT3(1.0f,  1.0f,  1.0f)},
	{XMFLOAT3(1.0f,  -1.0f,  1.0f)},
};

std::vector<DirectX::XMFLOAT3> cubeColours = {
	{XMFLOAT3(0.1f, 0.1f, 0.1f)},
	{XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(1.0f, 1.0f, 0.0f)},
	{XMFLOAT3(0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, 1.0f)},
};

std::vector<DirectX::XMFLOAT2> cubeUVs = {
	{XMFLOAT2(0.0f, 0.0f)},
	{XMFLOAT2(1.0f, 0.0f)},
	{XMFLOAT2(0.0f, 1.0f)},
	{XMFLOAT2(1.0f, 1.0f)},
	{XMFLOAT2(0.0f, 1.0f)},
	{XMFLOAT2(1.0f, 1.0f)},
	{XMFLOAT2(0.0f, 0.0f)},
	{XMFLOAT2(1.0f, 0.0f)},
};

std::vector<int> cubeIndices = {
	0,1,2,0,2,3,
	4,6,5,4,7,6,
	4,5,1,4,1,0,
	3,2,6,3,6,7,
	1,5,6,1,6,2,
	4,0,3,4,3,7,
};

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

std::vector<int> triangleIndices = { 0,1,2,5,4,3 };

void ExampleScene::makePlane(D3DContext* d3dContext) {
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

	planeMeshData = new MeshData({ planePositionsVB, planeColoursVB, planeNormalsVB }, planeIndices);
	planeMesh = new Mesh(d3dContext, *planeMeshData, *baseColourVertexShader);
	Object plane = Object(d3dContext, XMVectorSet(-2, 0, 0, 0), 0.0f,  *planeMesh, baseColourMaterial);
	scene.objects.push_back(plane);
}

void ExampleScene::createHexMesh(D3DContext* d3dContext) {
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
	hexMeshData = new MeshData({hexPrismPosVB, hexPrismColoursVB, hexPrismNormalsVB}, hexPrismIndices);
	hexMesh = new Mesh(d3dContext, *hexMeshData, *baseColourVertexShader);
}

void ExampleScene::setupLighting() {
	scene.lightData.pointLights[scene.lightData.numPointLights].pos = DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f);
	scene.lightData.pointLights[scene.lightData.numPointLights].colour = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	scene.lightData.pointLights[scene.lightData.numPointLights].radius = 20.0f;
	scene.lightData.numPointLights++;

	XMStoreFloat3(&scene.lightData.directionalLight.direction, DirectX::XMVector3Normalize(DirectX::XMVectorSet(1.0f, -1.0f, 0.1f, 0.0f)));
	scene.lightData.directionalLight.colour = DirectX::XMFLOAT3(100.0f, 98.0f, 98.0f);
}

void ExampleScene::setupCamera() {
	scene.eyePosition = XMVectorSet(0, 10, -10, 1);
	scene.focusPoint = XMVectorSet(0, 0, 0, 1);
}

ExampleScene::ExampleScene(D3DContext* d3dContext) {
	ID3D11Device* d3dDevice = d3dContext->d3dDevice;

	baseColourVertexShader = new VertexShader(d3dContext, (const void*)g_BaseColourVertexShader, sizeof(g_BaseColourVertexShader));
	baseColourPixelShader = new PixelShader(d3dContext, g_ps, sizeof(g_ps));
	baseColourMaterial = new Material(*baseColourPixelShader);

	makePlane(d3dContext);
	createHexMesh(d3dContext);

	int width = 30;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			HexCoord c = {i-width/2, j-width/2};
			scene.objects.push_back(Object(d3dContext, XMVectorSet(c.cartesian().x, World::getHeight(c), c.cartesian().y, 0.0f), 90.0f, *hexMesh, baseColourMaterial));
		}
	}

	setupLighting();
	setupCamera();
}

ExampleScene::~ExampleScene() {
	delete baseColourMaterial;
	delete baseColourPixelShader;
	delete baseColourVertexShader;

	delete planeMeshData;
	delete planeMesh;

	delete hexMeshData;
	delete hexMesh;
}

void ExampleScene::Tick(float deltaTime) {
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
}