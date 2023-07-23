#pragma once

#include "DirectXTemplatePCH.h"

#include "renderer/D3DContext.h"
#include "renderer/Scene.h"

class ExampleScene {
public:
	ExampleScene(D3DContext* d3dContext);
	~ExampleScene();
	void Tick(float deltaTime);

	Scene scene;

private:
	Material* baseColourMaterial = nullptr;
	Material* texturedMaterial = nullptr;
	Texture2D* gravelTexture = nullptr;
	PixelShader* baseColourPixelShader = nullptr;
	PixelShader* texturedPixelShader = nullptr;
	VertexShader* baseColourVertexShader = nullptr;
	VertexShader* texturedVertexShader = nullptr;

	MeshData* colouredCubeMeshData = nullptr;
	Mesh* colouredCubeMesh = nullptr;
	Object* colouredCubeObj = nullptr;

	MeshData* dragonMeshData = nullptr;
	Mesh* dragonMesh = nullptr;
	Object* stanfordDragonObj = nullptr;

	MeshData* dragon2MD = nullptr;
	Mesh* dragon2M = nullptr;
	Object* dragon2O = nullptr;

	MeshData* colouredTriangleMeshData = nullptr;
	Mesh* colouredTriangleMesh = nullptr;
	Object* colouredTriangleObj = nullptr;

	MeshData* texturedCubeMeshData = nullptr;
	Mesh* texturedCubeMesh = nullptr;
	Object* texturedCubeObj = nullptr;

	MeshData* planeMeshData;
	Mesh* planeMesh;
	Object* planeObject;
};


struct VertexPosColor {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

struct VertexPosUV {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};
