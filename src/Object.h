#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Material.h"

class IObject {
public:
	virtual void RenderObject(D3DContext* d3dContext, float deltaTime) = 0;
	virtual DirectX::XMVECTOR& GetPos() = 0;
	virtual DirectX::XMMATRIX GetModelMatrix() = 0;
	IObject(UINT numIndices, ID3D11InputLayout* inputLayout, ID3D11VertexShader* vs, Material* material, DirectX::XMVECTOR pos, float angle);

	UINT numIndices;
	ID3D11Buffer* d3dVertexBuffer = nullptr;
	ID3D11Buffer* d3dIndexBuffer = nullptr;
	ID3D11InputLayout* d3dInputLayout = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;

	Material* material;
	DirectX::XMVECTOR pos;
	float angle;
};

template <typename T>
class Object : public IObject {
public:
	Object(D3DContext* d3dContext, DirectX::XMVECTOR pos, 
		UINT numVertices, T* vertices, UINT numIndices, int* indices, 
		ID3D11VertexShader* vs, ID3D11InputLayout* inputLayout, Material* material);
	int CreateVertexBuffer(D3DContext* d3dContext, UINT numVertices, T* vertices);
	int CreateIndexBuffer(D3DContext* d3dContext, UINT numIndices, int* indices);
	DirectX::XMMATRIX GetModelMatrix();
	virtual DirectX::XMVECTOR& GetPos();
	void RenderObject(D3DContext* d3dContext, float deltaTime);
};


// OBJECT<T> IMPL

template <typename T>
Object<T>::Object(D3DContext* d3dContext, DirectX::XMVECTOR pos, 
	UINT numVertices, T* vertices, UINT numIndices, int* indices,
	ID3D11VertexShader* vs, ID3D11InputLayout* inputLayout, Material* material)
	: IObject(numIndices, 
		inputLayout,
		vs,
		material,
		pos,
		0.0f)
{
	CreateVertexBuffer(d3dContext, numVertices, vertices);
	CreateIndexBuffer(d3dContext, numIndices, indices);

	angle = 0.0f;
}

template <typename T>
int Object<T>::CreateVertexBuffer(D3DContext* d3dContext, UINT numVertices, T* vertices) {
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = vertices;

	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &d3dVertexBuffer));

	return 0;
}

template <typename T>
int Object<T>::CreateIndexBuffer(D3DContext* d3dContext, UINT numIndices, int* indices) {
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(int) * numIndices;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = indices;

	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&indexBufferDesc, &resourceData, &d3dIndexBuffer));

	return 0;
}

template <typename T>
DirectX::XMMATRIX Object<T>::GetModelMatrix() {
	return DirectX::XMMatrixTranslationFromVector(pos);
}

template <typename T>
DirectX::XMVECTOR& Object<T>::GetPos() {
	return pos;
}

void RenderObject(D3DContext* d3dContext, float deltaTime, UINT vertexStride, IObject& obj);

template <typename T>
void Object<T>::RenderObject(D3DContext* d3dContext, float deltaTime) {
	const UINT vertexStride = sizeof(T);
	::RenderObject(d3dContext, deltaTime, vertexStride, *this);
}
