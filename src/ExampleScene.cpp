#include "DirectXTemplatePCH.h"

#include "ExampleScene.h"
#include "Object.h"
#include "Material.h"

#include "..\include\VertexShader.h"
#include "..\include\PixelShader.h"

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

WORD cubeIndices[36] = {
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

WORD triangleIndices[6] = { 0,1,2,2,1,0 };

static Material* baseColourMaterial = nullptr;
static ID3D11PixelShader* baseColourPixelShader = nullptr;
static ID3D11VertexShader* vertexShader = nullptr;
static ID3D11InputLayout* d3dInputLayout = nullptr;

Scene buildExampleScene(D3DContext* d3dContext) {
	Scene scene = Scene();

	ID3D11Device* d3dDevice = d3dContext->d3dDevice;

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Color), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	HRASSERT(d3dDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), g_vs, sizeof(g_vs), &d3dInputLayout));

	HRASSERT(d3dDevice->CreateVertexShader(g_vs, sizeof(g_vs), nullptr, &vertexShader));
	HRASSERT(d3dDevice->CreatePixelShader(g_ps, sizeof(g_ps), nullptr, &baseColourPixelShader));

	baseColourMaterial = new Material(baseColourPixelShader);

	scene.objects.push_back(Object(d3dContext, XMMatrixIdentity(), (UINT)8, cubeVertices, (UINT)36, cubeIndices, vertexShader, d3dInputLayout, baseColourMaterial));
	scene.objects.push_back(Object(d3dContext, XMMatrixTranslation(4, 0, 2), (UINT)8, cubeVertices, (UINT)36, cubeIndices, vertexShader, d3dInputLayout, baseColourMaterial));
	scene.objects.push_back(Object(d3dContext, XMMatrixTranslation(-2, 1, 2), (UINT)8, cubeVertices, (UINT)36, cubeIndices, vertexShader, d3dInputLayout, baseColourMaterial));
	scene.objects.push_back(Object(d3dContext, XMMatrixTranslation(-4, 1, -2), (UINT)8, cubeVertices, (UINT)36, cubeIndices, vertexShader, d3dInputLayout, baseColourMaterial));
	scene.objects.push_back(Object(d3dContext, XMMatrixTranslation(-2, -3, 3), (UINT)8, cubeVertices, (UINT)36, cubeIndices, vertexShader, d3dInputLayout, baseColourMaterial));
	scene.objects.push_back(Object(d3dContext, XMMatrixTranslation(0, 1, 3), (UINT)8, cubeVertices, (UINT)36, cubeIndices, vertexShader, d3dInputLayout, baseColourMaterial));
	scene.objects.push_back(Object(d3dContext, XMMatrixTranslation(2, -2, 1), _countof(triangleVertices), triangleVertices, _countof(triangleIndices), triangleIndices, vertexShader, d3dInputLayout, baseColourMaterial));

	return scene;
}

void cleanupExampleScene() {
	SafeRelease(baseColourPixelShader);
	SafeRelease(vertexShader);
	SafeRelease(d3dInputLayout);
	delete baseColourMaterial;
}