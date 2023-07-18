#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Shader.h"

#include <vector>

class MeshData {
public:
	MeshData(std::vector<uint8_t> inVerts, std::vector<int> inIndices, std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout, size_t vertWidth);
	std::vector<uint8_t> verts;
	std::vector<int> indices;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
	size_t vertexStride;
};

class Mesh {
public:
	Mesh(D3DContext* d3dContext, MeshData& meshData, VertexShader& vs);

	void CreateVertexBuffer(D3DContext* d3dContext);
	void CreateIndexBuffer(D3DContext* d3dContext);
	void CreateInputLayout(D3DContext* d3dContext);

	MeshData& meshData;
	VertexShader& vertexShader;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
};

