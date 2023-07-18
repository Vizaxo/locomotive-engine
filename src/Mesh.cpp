#include "DirectXTemplatePCH.h"

#include "Mesh.h"

MeshData::MeshData(std::vector<VertexBuffer> vertexBuffers, std::vector<int> inIndices)
	: vertexBuffers(vertexBuffers), indices(inIndices)
{}

Mesh::Mesh(D3DContext* d3dContext, MeshData& meshData, VertexShader& vs)
	: meshData(meshData), vertexShader(vs)
{
	InitialiseVertexBuffers(d3dContext);
	CreateIndexBuffer(d3dContext);
	CreateInputLayout(d3dContext);
}

void Mesh::InitialiseVertexBuffers(D3DContext* d3dContext) {
	for (VertexBuffer& vertexBuffer : meshData.vertexBuffers)
		vertexBuffer.Initialise(d3dContext);
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
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
	for (VertexBuffer& vertexBuffer : meshData.vertexBuffers)
		for (D3D11_INPUT_ELEMENT_DESC& desc : vertexBuffer.inputDesc)
			inputDesc.push_back(desc);

	inputLayout = vertexShader.CreateInputLayout(d3dContext, inputDesc);
}
