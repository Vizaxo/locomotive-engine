#include "DirectXTemplatePCH.h"

#include "Mesh.h"

MeshData::MeshData(std::vector<uint8_t> inVerts, std::vector<int> inIndices, std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc, size_t vertWidth)
	: verts(inVerts), indices(inIndices), inputDesc(inputDesc), vertexStride(vertWidth)
{}

Mesh::Mesh(D3DContext* d3dContext, MeshData& meshData, VertexShader& vs)
	: meshData(meshData), vertexShader(vs)
{
	CreateVertexBuffer(d3dContext);
	CreateIndexBuffer(d3dContext);
	CreateInputLayout(d3dContext);
}

void Mesh::CreateVertexBuffer(D3DContext* d3dContext) {
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = meshData.verts.size();
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = meshData.verts.data();

	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &vertexBuffer));
}

void Mesh::CreateIndexBuffer(D3DContext* d3dContext) {
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(int) * meshData.indices.size();
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = meshData.indices.data();

	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&indexBufferDesc, &resourceData, &indexBuffer));
}

void Mesh::CreateInputLayout(D3DContext* d3dContext) {
	inputLayout = vertexShader.CreateInputLayout(d3dContext, meshData.inputDesc);
}
