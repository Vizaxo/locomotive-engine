#pragma once

#include "DirectXTemplatePCH.h"

#include "rhi/RHI.h"
#include "renderer/Scene.h"
#include "renderer/Material.h"
#include "renderer/Mesh.h"
#include "renderer/Object.h"

class ExampleScene {
public:
	ExampleScene(D3DContext* d3dContext);
	~ExampleScene();
	void Tick(float deltaTime);

	Scene scene;

private:
	Material* baseColourMaterial = nullptr;
	PixelShader* baseColourPixelShader = nullptr;
	VertexShader* baseColourVertexShader = nullptr;

	MeshData* planeMeshData;
	Mesh* planeMesh;
	Object* planeObject;

	MeshData* hexMeshData;
	Mesh* hexMesh;
	Object* hexObject;
};


struct VertexPosColor {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

struct VertexPosUV {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};
