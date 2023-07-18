#include "DirectXTemplatePCH.h"

#include "ExampleScene.h"
#include "Object.h"
#include "Material.h"
#include "Texture2D.h"
#include "ModelLoader.h"
#include "Mesh.h"

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
static VertexShader* baseColourVSData = nullptr;
static VertexShader* texturedVSData = nullptr;

static MeshData* colouredCubeMeshData = nullptr;
static Mesh* colouredCubeMesh = nullptr;
static Object* colouredCubeObj = nullptr;

static MeshData* dragonMeshData = nullptr;
static Mesh* dragonMesh = nullptr;
static Object* stanfordDragonObj = nullptr;

static MeshData* colouredTriangleMeshData = nullptr;
static Mesh* colouredTriangleMesh = nullptr;
static Object* colouredTriangleObj = nullptr;

static MeshData* texturedCubeMeshData = nullptr;
static Mesh* texturedCubeMesh = nullptr;
static Object* texturedCubeObj = nullptr;

Scene buildExampleScene(D3DContext* d3dContext) {
	Scene scene = Scene();

	ID3D11Device* d3dDevice = d3dContext->d3dDevice;

	std::vector<D3D11_INPUT_ELEMENT_DESC> baseColourInputDesc = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Color), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	HRASSERT(d3dDevice->CreateVertexShader(g_BaseColourVertexShader, sizeof(g_BaseColourVertexShader), nullptr, &baseColourVertexShader));
	baseColourVSData = new VertexShader((const void*)g_BaseColourVertexShader, sizeof(g_BaseColourVertexShader), baseColourVertexShader);
	HRASSERT(d3dDevice->CreatePixelShader(g_ps, sizeof(g_ps), nullptr, &baseColourPixelShader));

	std::vector<D3D11_INPUT_ELEMENT_DESC> texturedInputDesc = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosUV,Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosUV,UV), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	HRASSERT(d3dDevice->CreateVertexShader(g_TexturedVertexShader, sizeof(g_TexturedVertexShader), nullptr, &texturedVertexShader));
	texturedVSData = new VertexShader((const void*)g_TexturedVertexShader, sizeof(g_TexturedVertexShader), texturedVertexShader);
	HRASSERT(d3dDevice->CreatePixelShader(g_texturedPixelShader, sizeof(g_texturedPixelShader), nullptr, &texturedPixelShader));

	gravelTexture = new Texture2D(d3dContext, (char*)"resources\\textures\\Gravel_001_BaseColor.jpg");

	baseColourMaterial = (new Material(baseColourPixelShader));
	texturedMaterial = (new Material(texturedPixelShader))->setTexture(d3dContext, gravelTexture);

	std::vector<uint8_t> colouredCubeVerts = std::vector((uint8_t*)cubeVertices, (uint8_t*)cubeVertices + sizeof(cubeVertices));
	colouredCubeMeshData = new MeshData(colouredCubeVerts, std::vector(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(int)), baseColourInputDesc, sizeof(VertexPosColor));
	colouredCubeMesh = new Mesh(d3dContext, *colouredCubeMeshData, *baseColourVSData);
	colouredCubeObj = new Object(d3dContext, XMVectorSet(-2, 0, 0, 0), 0.0f,  *colouredCubeMesh, baseColourMaterial);
	scene.objects.push_back(colouredCubeObj);

	std::vector<uint8_t> colouredTriangleVerts = std::vector((uint8_t*)triangleVertices, (uint8_t*)triangleVertices + sizeof(triangleVertices));
	colouredTriangleMeshData = new MeshData(colouredTriangleVerts, std::vector(triangleIndices, triangleIndices + sizeof(triangleIndices) / sizeof(int)), baseColourInputDesc, sizeof(VertexPosColor));
	colouredTriangleMesh = new Mesh(d3dContext, *colouredTriangleMeshData, *baseColourVSData);
	colouredTriangleObj = new Object(d3dContext, XMVectorSet(2, 0, 0, 0), 0.0f,  *colouredTriangleMesh, baseColourMaterial);
	scene.objects.push_back(colouredTriangleObj);

	std::vector<uint8_t> texturedCubeVerts = std::vector((uint8_t*)texturedCube, (uint8_t*)texturedCube + sizeof(texturedCube));
	texturedCubeMeshData = new MeshData(texturedCubeVerts, std::vector(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(int)), texturedInputDesc, sizeof(VertexPosUV));
	texturedCubeMesh = new Mesh(d3dContext, *texturedCubeMeshData, *texturedVSData);
	texturedCubeObj = new Object(d3dContext, XMVectorSet(3, -1, 0, 0), 0.0f,  *texturedCubeMesh, texturedMaterial);
	scene.objects.push_back(texturedCubeObj);

	auto res = ModelLoader::LoadModel(L"resources\\models\\stanford_dragon_res3.ply");
	//auto res = ModelLoader::LoadModel(L"resources\\models\\stanford_dragon_vrip.ply");
	if (std::string* msg = std::get_if<std::string>(&res); msg) {
		assert(false);
	} else if (auto* dragon = std::get_if<MeshData>(&res); dragon) {
		std::vector<VertexPosColor> colouredVerts;

		// Scale up verts
		for (long i = 0; i < dragon->verts.size(); i+=sizeof(DirectX::XMFLOAT3)) {
			DirectX::XMFLOAT3* v = (DirectX::XMFLOAT3*)&dragon->verts[i];
			XMStoreFloat3(v, DirectX::XMVectorScale(XMLoadFloat3(v), 10.0f));
			colouredVerts.push_back({ *v , {} });
		}

		// Calculate normals
		for (int i = 0; i < dragon->indices.size(); i+=3) {
			VertexPosColor& v1 = colouredVerts[dragon->indices[i]];
			VertexPosColor& v2 = colouredVerts[dragon->indices[i+1]];
			VertexPosColor& v3 = colouredVerts[dragon->indices[i+2]];
			DirectX::XMVECTOR faceNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(XMLoadFloat3(&v2.Position), XMLoadFloat3(&v1.Position))
				, DirectX::XMVectorSubtract(XMLoadFloat3(&v3.Position), XMLoadFloat3(&v1.Position))));
			DirectX::XMFLOAT3 faceNormalV3;
			XMStoreFloat3(&faceNormalV3, faceNormal);
			v1.Color = faceNormalV3;
			v2.Color = faceNormalV3;
			v3.Color = faceNormalV3;
		}

		dragonMeshData = new MeshData(std::vector((uint8_t*)colouredVerts.data(), (uint8_t*)colouredVerts.data() + colouredVerts.size()*sizeof(VertexPosColor)), std::vector(dragon->indices.data(), dragon->indices.data() + dragon->indices.size()), baseColourInputDesc, sizeof(VertexPosColor));
		dragonMesh = new Mesh(d3dContext, *dragonMeshData, *baseColourVSData);
		stanfordDragonObj = new Object(d3dContext, XMVectorSet(0, 1, 0, 0), 0.0f, *dragonMesh, baseColourMaterial);

		scene.objects.push_back(stanfordDragonObj);
	}

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
	delete colouredCubeMesh;
	delete colouredTriangleObj;
	delete texturedCubeObj;
}