#include "ExampleScene.h"
#include "Utils.h"
#include "ImGuiUtils.h"
#include "renderer/Object.h"
#include "renderer/Material.h"
#include "renderer/Texture2D.h"
#include "editor/ModelLoader.h"
#include "renderer/Mesh.h"
#include "renderer/VertexBuffer.h"

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
	{XMFLOAT3(0.0f, 0.0f, 0.0f)},
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

std::vector<int> triangleIndices = { 0,1,2,5,4,3 };

ExampleScene::ExampleScene(D3DContext* d3dContext) {
	ID3D11Device* d3dDevice = d3dContext->d3dDevice;

	baseColourVertexShader = new VertexShader(d3dContext, (const void*)g_BaseColourVertexShader, sizeof(g_BaseColourVertexShader));
	baseColourPixelShader = new PixelShader(d3dContext, g_ps, sizeof(g_ps));

	texturedVertexShader = new VertexShader(d3dContext, (const void*)g_TexturedVertexShader, sizeof(g_TexturedVertexShader));
	texturedPixelShader = new PixelShader(d3dContext, g_texturedPixelShader, sizeof(g_texturedPixelShader));

	gravelTexture = new Texture2D(d3dContext, (char*)"resources\\textures\\Gravel_001_BaseColor.jpg");

	baseColourMaterial = new Material(*baseColourPixelShader);
	texturedMaterial = (new Material(*texturedPixelShader))->setTexture(d3dContext, gravelTexture);

	VertexBuffer cubePositionsVB = CreateVertexBuffer(cubePositions,
		{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		0);

	VertexBuffer cubeColoursVB = CreateVertexBuffer(cubeColours,
		{ {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		1);

	// Calculate normals
	std::vector<DirectX::XMFLOAT3> cubeNormals = std::vector<DirectX::XMFLOAT3>(cubePositions.size());
	for (size_t i = 0; i < cubeIndices.size(); i += 3) {
		DirectX::XMFLOAT3* v1 = (DirectX::XMFLOAT3*)&cubePositions[cubeIndices[i]];
		DirectX::XMFLOAT3* v2 = (DirectX::XMFLOAT3*)&cubePositions[cubeIndices[i + 1]];
		DirectX::XMFLOAT3* v3 = (DirectX::XMFLOAT3*)&cubePositions[cubeIndices[i + 2]];
		DirectX::XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(XMLoadFloat3(v2), XMLoadFloat3(v1))
			, DirectX::XMVectorSubtract(XMLoadFloat3(v3), XMLoadFloat3(v1))));
		DirectX::XMFLOAT3 faceNormalV3;
		XMStoreFloat3(&faceNormalV3, faceNormal);

		cubeNormals[cubeIndices[i]] = faceNormalV3;
		cubeNormals[cubeIndices[i+1]] = faceNormalV3;
		cubeNormals[cubeIndices[i+2]] = faceNormalV3;
	}
	VertexBuffer cubeNormalsVB = CreateVertexBuffer(cubeNormals,
		{ {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		2);

	colouredCubeMeshData = new MeshData({ cubePositionsVB, cubeColoursVB, cubeNormalsVB }, cubeIndices);
	colouredCubeMesh = new Mesh(d3dContext, *colouredCubeMeshData, *baseColourVertexShader);
	colouredCubeObj = new Object(d3dContext, XMVectorSet(-2, 0, 0, 0), 0.0f,  *colouredCubeMesh, baseColourMaterial);
	scene.objects.push_back(colouredCubeObj);

	VertexBuffer colouredTriangleVB = CreateVertexBuffer(triangleVertices,
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Color), D3D11_INPUT_PER_VERTEX_DATA, 0}
		},
		0);

	// Calculate normals
	std::vector<DirectX::XMFLOAT3> triangleNormals = std::vector<DirectX::XMFLOAT3>(triangleVertices.size());
	for (size_t i = 0; i < triangleIndices.size(); i += 3) {
		DirectX::XMFLOAT3* v1 = (DirectX::XMFLOAT3*)&triangleVertices[triangleIndices[i]];
		DirectX::XMFLOAT3* v2 = (DirectX::XMFLOAT3*)&triangleVertices[triangleIndices[i + 1]];
		DirectX::XMFLOAT3* v3 = (DirectX::XMFLOAT3*)&triangleVertices[triangleIndices[i + 2]];
		DirectX::XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(XMLoadFloat3(v2), XMLoadFloat3(v1))
			, DirectX::XMVectorSubtract(XMLoadFloat3(v3), XMLoadFloat3(v1))));
		DirectX::XMFLOAT3 faceNormalV3;
		XMStoreFloat3(&faceNormalV3, faceNormal);

		triangleNormals[triangleIndices[i]] = faceNormalV3;
		triangleNormals[triangleIndices[i+1]] = faceNormalV3;
		triangleNormals[triangleIndices[i+2]] = faceNormalV3;
	}
	VertexBuffer triangleNormalsVB = CreateVertexBuffer(triangleNormals,
		{ {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		2);

	colouredTriangleMeshData = new MeshData({ colouredTriangleVB, triangleNormalsVB }, triangleIndices);
	colouredTriangleMesh = new Mesh(d3dContext, *colouredTriangleMeshData, *baseColourVertexShader);
	colouredTriangleObj = new Object(d3dContext, XMVectorSet(2, 0, 0, 0), 0.0f,  *colouredTriangleMesh, baseColourMaterial);
	scene.objects.push_back(colouredTriangleObj);

	VertexBuffer cubeUVsVB = CreateVertexBuffer(cubeUVs,
		{ {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
		1);
	texturedCubeMeshData = new MeshData({ cubePositionsVB, cubeUVsVB, cubeNormalsVB }, cubeIndices);
	texturedCubeMesh = new Mesh(d3dContext, *texturedCubeMeshData, *texturedVertexShader);
	texturedCubeObj = new Object(d3dContext, XMVectorSet(3, -1, 0, 0), 0.0f,  *texturedCubeMesh, texturedMaterial);
	scene.objects.push_back(texturedCubeObj);

	auto res = ModelLoader::LoadModel(L"resources\\models\\stanford_dragon_res3.ply");
	//auto res = ModelLoader::LoadModel(L"resources\\models\\stanford_dragon_vrip.ply");
	if (std::string* msg = std::get_if<std::string>(&res); msg) {
		assert(false);
	} else if (auto* dragon = std::get_if<MeshData>(&res); dragon) {
		// Scale up verts
		for (long i = 0; i < dragon->vertexBuffers[0].verts.size(); i+=sizeof(DirectX::XMFLOAT3)) {
			DirectX::XMFLOAT3* v = (DirectX::XMFLOAT3*)&dragon->vertexBuffers[0].verts[i];
			XMStoreFloat3(v, DirectX::XMVectorScale(XMLoadFloat3(v), 10.0f));
		}

		std::vector<DirectX::XMFLOAT3> colours = std::vector<DirectX::XMFLOAT3>(dragon->vertexBuffers[0].verts.size() / dragon->vertexBuffers[0].stride);
		for (size_t i = 0; i < dragon->indices.size(); i += 3) {
			DirectX::XMFLOAT3 colour = { 0.9f, 0.9f, 0.9f };
			colours[dragon->indices[i]] = colour;
			colours[dragon->indices[i+1]] = colour;
			colours[dragon->indices[i+2]] = colour;
		}
		VertexBuffer dragonColours = CreateVertexBuffer(colours,
			{ {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
			1);
		dragon->vertexBuffers.push_back(dragonColours);

		// Calculate normals
		std::vector<DirectX::XMFLOAT3> normals = std::vector<DirectX::XMFLOAT3>(dragon->vertexBuffers[0].verts.size() / dragon->vertexBuffers[0].stride);
		for (size_t i = 0; i < dragon->indices.size(); i += 3) {
			size_t stride = dragon->vertexBuffers[0].stride;

			DirectX::XMFLOAT3* v1 = (DirectX::XMFLOAT3*)&dragon->vertexBuffers[0].verts[dragon->indices[i]*stride];
			DirectX::XMFLOAT3* v2 = (DirectX::XMFLOAT3*)&dragon->vertexBuffers[0].verts[dragon->indices[i+1]*stride];
			DirectX::XMFLOAT3* v3 = (DirectX::XMFLOAT3*)&dragon->vertexBuffers[0].verts[dragon->indices[i+2]*stride];
			DirectX::XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(XMLoadFloat3(v2), XMLoadFloat3(v1))
				, DirectX::XMVectorSubtract(XMLoadFloat3(v3), XMLoadFloat3(v1))));
			DirectX::XMFLOAT3 faceNormalV3;
			XMStoreFloat3(&faceNormalV3, faceNormal);

			normals[dragon->indices[i]] = faceNormalV3;
			normals[dragon->indices[i+1]] = faceNormalV3;
			normals[dragon->indices[i+2]] = faceNormalV3;
		}
		VertexBuffer dragonNormals = CreateVertexBuffer(normals,
			{ {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} },
			2);
		dragon->vertexBuffers.push_back(dragonNormals);

		dragonMeshData = new MeshData(*dragon);
		dragonMesh = new Mesh(d3dContext, *dragonMeshData, *baseColourVertexShader);
		stanfordDragonObj = new Object(d3dContext, XMVectorSet(0, 1, 0, 0), 0.0f, *dragonMesh, baseColourMaterial);

		scene.objects.push_back(stanfordDragonObj);
	}

	scene.lightData.lights[scene.lightData.numLights].pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scene.lightData.lights[scene.lightData.numLights].colour = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	scene.lightData.lights[scene.lightData.numLights].radius = 5.0f;
	scene.lightData.numLights++;

	scene.lightData.lights[scene.lightData.numLights].pos = DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f);
	scene.lightData.lights[scene.lightData.numLights].colour = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	scene.lightData.lights[scene.lightData.numLights].radius = 5.0f;
	scene.lightData.numLights++;

	scene.lightData.lights[scene.lightData.numLights].pos = DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f);
	scene.lightData.lights[scene.lightData.numLights].colour = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	scene.lightData.lights[scene.lightData.numLights].radius = 20.0f;
	scene.lightData.numLights++;
}

ExampleScene::~ExampleScene() {
	delete baseColourMaterial;
	delete texturedMaterial;
	delete gravelTexture;
	delete baseColourPixelShader;
	delete texturedPixelShader;
	delete baseColourVertexShader;
	delete texturedVertexShader;

	delete colouredCubeMeshData;
	delete colouredCubeMesh;
	delete colouredCubeObj;

	delete dragonMeshData;
	delete dragonMesh;
	delete stanfordDragonObj;

	delete colouredTriangleMeshData;
	delete colouredTriangleMesh;
	delete colouredTriangleObj;

	delete texturedCubeMeshData;
	delete texturedCubeMesh;
	delete texturedCubeObj;
}

void ExampleScene::Tick(float deltaTime) {
	{
		ImGui::Begin("Objects");                          // Create a window called "Hello, world!" and append into it.
		if (scene.objects.size() >= 1)
			MkSliderV3("Coloured cube", scene.objects[0]->GetPos(), -10.0f, 10.0f);
		if (scene.objects.size() >= 2)
			MkSliderV3("Coloured triangle", scene.objects[1]->GetPos(), -2.0f, 2.0f);
		if (scene.objects.size() >= 3)
			MkSliderV3("Textured cube", scene.objects[2]->GetPos(), -2.0f, 2.0f);
		if (scene.objects.size() >= 3)
			MkSliderV3("Dragon", scene.objects[3]->GetPos(), -5.0f, 5.0f);
		ImGui::End();
	}

	for (Object* obj : scene.objects) {
		obj->angle += 90.f * deltaTime;
	}
}