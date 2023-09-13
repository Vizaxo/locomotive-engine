#pragma once

#include "DirectXTemplatePCH.h"

#include "rhi/RHI.h"
#include "renderer/Scene.h"
#include "renderer/Material.h"
#include "renderer/Mesh.h"
#include "renderer/Object.h"
#include "Application.h"

class ExampleApplication : public Application {
	void init(D3DContext* d3dContext);
	void tick(float deltaTime);
	void cleanup();
	Scene& getScene();

	void makePlane(D3DContext* d3dContext);
	void createHexMesh(D3DContext* d3dContext);
	void setupLighting();
	void setupCamera();

	Scene scene;

private:
	Material* baseColourMaterial = nullptr;
	PixelShader* baseColourPixelShader = nullptr;
	VertexShader* baseColourVertexShader = nullptr;

	MeshData* planeMeshData;
	Mesh* planeMesh;

	MeshData* hexMeshData;
	Mesh* hexMesh;
};


struct VertexPosColor {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

struct VertexPosUV {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};