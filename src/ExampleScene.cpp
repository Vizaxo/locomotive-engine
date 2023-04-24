#include "DirectXTemplatePCH.h"

#include "ExampleScene.h"
#include "Object.h"
#include "Material.h"
#include "Texture2D.h"
#include "ModelLoader.h"

#include "BaseColourVertexShader.h"
#include "BaseColourPixelShader.h"
#include "TexturedPixelShader.h"
#include "TexturedVertexShader.h"

using namespace DirectX;

VertexPosColor cubeVertices[8] = {
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
	{XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
	{XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
};

VertexPosUV texturedCube[8] = {
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
	{XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)},
	{XMFLOAT3(1.0f,  -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)},
	{XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f)},
	{XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f)},
	{XMFLOAT3(1.0f,  -1.0f,  1.0f), XMFLOAT2(1.0f, 0.0f)},
};

int cubeIndices[36] = {
	0,1,2,0,2,3,
	4,6,5,4,7,6,
	4,5,1,4,1,0,
	3,2,6,3,6,7,
	1,5,6,1,6,2,
	4,0,3,4,3,7,
};

VertexPosColor triangleVertices[3] = {
	{XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(0.5f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
};

int triangleIndices[6] = { 0,1,2,2,1,0 };

static Material* baseColourMaterial = nullptr;
static Material* texturedMaterial = nullptr;
static Texture2D* gravelTexture = nullptr;
static ID3D11PixelShader* baseColourPixelShader = nullptr;
static ID3D11PixelShader* texturedPixelShader = nullptr;
static ID3D11VertexShader* baseColourVertexShader = nullptr;
static ID3D11VertexShader* texturedVertexShader = nullptr;
static ID3D11InputLayout* baseColourInputLayout = nullptr;
static ID3D11InputLayout* texturedInputLayout = nullptr;

static IObject* colouredCubeObj = nullptr;
static IObject* stanfordDragonObj = nullptr;
static IObject* colouredTriangleObj = nullptr;
static IObject* texturedCubeObj = nullptr;

Scene buildExampleScene(D3DContext* d3dContext) {
	Scene scene = Scene();
	scene.Initialise(d3dContext);

	ID3D11Device* d3dDevice = d3dContext->d3dDevice;

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Color), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	HRASSERT(d3dDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), g_BaseColourVertexShader, sizeof(g_BaseColourVertexShader), &baseColourInputLayout));
	HRASSERT(d3dDevice->CreateVertexShader(g_BaseColourVertexShader, sizeof(g_BaseColourVertexShader), nullptr, &baseColourVertexShader));
	HRASSERT(d3dDevice->CreatePixelShader(g_ps, sizeof(g_ps), nullptr, &baseColourPixelShader));

	D3D11_INPUT_ELEMENT_DESC texturedVertexLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosUV,Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosUV,UV), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	HRASSERT(d3dDevice->CreateInputLayout(texturedVertexLayoutDesc, _countof(texturedVertexLayoutDesc), g_TexturedVertexShader, sizeof(g_TexturedVertexShader), &texturedInputLayout));
	HRASSERT(d3dDevice->CreatePixelShader(g_texturedPixelShader, sizeof(g_texturedPixelShader), nullptr, &texturedPixelShader));
	HRASSERT(d3dDevice->CreateVertexShader(g_TexturedVertexShader, sizeof(g_TexturedVertexShader), nullptr, &texturedVertexShader));

	gravelTexture = new Texture2D(d3dContext, (char*)"resources\\textures\\Gravel_001_BaseColor.jpg");

	baseColourMaterial = (new Material(baseColourPixelShader));
	texturedMaterial = (new Material(texturedPixelShader))->setTexture(d3dContext, gravelTexture);

	colouredCubeObj = new Object(d3dContext, XMVectorSet(-2, 0, 0, 0), (UINT)8, cubeVertices, (UINT)36, cubeIndices, baseColourVertexShader, baseColourInputLayout, baseColourMaterial);
	colouredTriangleObj = new Object(d3dContext, XMVectorSet(2, -2, 1, 0), _countof(triangleVertices), triangleVertices, _countof(triangleIndices), triangleIndices, baseColourVertexShader, baseColourInputLayout, baseColourMaterial);
	texturedCubeObj = new Object<VertexPosUV>(d3dContext, XMVectorSet(4, 0, 2, 0), (UINT)8, texturedCube, (UINT)36, cubeIndices, texturedVertexShader, texturedInputLayout, texturedMaterial);

	auto res = ModelLoader::LoadModel(L"resources\\models\\stanford_dragon_res3.ply");
	if (std::string* msg = std::get_if<std::string>(&res); msg) {
		assert(false);
	} else if (auto* dragon = std::get_if<Model>(&res); dragon) {
		std::vector<VertexPosColor> colouredVerts;
		XMFLOAT3 v;
		for (long i = 0; i < dragon->verts.size(); i++) {
			XMStoreFloat3(&v, DirectX::XMVectorScale(XMLoadFloat3(&dragon->verts[i]), 10.0f));
			colouredVerts.push_back({v , DirectX::XMFLOAT3((float)(i % 10) / 10, (float)(i % 100) / 100, (float)(i % 1000) / 1000) });
		}
		stanfordDragonObj = new Object<VertexPosColor>(d3dContext, XMVectorSet(0, 0, 0, 0), colouredVerts.size(), colouredVerts.data(), dragon->indices.size(), dragon->indices.data(), baseColourVertexShader, baseColourInputLayout, baseColourMaterial);
		scene.objects.push_back(stanfordDragonObj);
	}

	scene.objects.push_back(colouredCubeObj);
	scene.objects.push_back(colouredTriangleObj);
	scene.objects.push_back(texturedCubeObj);

	return scene;
}

void cleanupExampleScene() {
	SafeRelease(baseColourPixelShader);
	SafeRelease(baseColourVertexShader);
	SafeRelease(baseColourInputLayout);
	SafeRelease(texturedPixelShader);
	SafeRelease(texturedVertexShader);
	SafeRelease(texturedInputLayout);
	delete baseColourMaterial;
	delete texturedMaterial;
	delete gravelTexture;
	delete colouredCubeObj;
	delete colouredTriangleObj;
	delete texturedCubeObj;
}