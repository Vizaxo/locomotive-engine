#pragma once

#include "PCH.h"

#include "resource-manager/ResourceManager.h"
#include "rhi/RHI.h"

class MeshData {
public:
	MeshData(std::vector<VertexBuffer> vertexBuffers, std::vector<int> inIndices);
	MeshData(MeshData& other);
	MeshData(MeshData&& other);
	std::vector<VertexBuffer> vertexBuffers;
	std::vector<int> indices;
};

class Mesh {
public:
	Mesh(D3DContext* d3dContext, MeshData& meshData, VertexShader& vs);

	void InitialiseVertexBuffers(D3DContext* d3dContext);
	void CreateIndexBuffer(D3DContext* d3dContext);
	void CreateInputLayout(D3DContext* d3dContext);

	MeshData& meshData;
	VertexShader& vertexShader;

	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
};

extern ResourceManager<Mesh> meshManager;
