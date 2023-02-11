#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Material.h"

class IObject {
public:
	virtual void RenderObject(D3DContext* d3dContext, float deltaTime) = 0;
	virtual DirectX::XMVECTOR& GetPos() = 0;
};

template <typename T>
class Object : public IObject {
public:
	Material* material;

	ID3D11Buffer* d3dVertexBuffer = nullptr;
	ID3D11Buffer* d3dIndexBuffer = nullptr;
	ID3D11InputLayout* d3dInputLayout = nullptr;

	UINT numIndices;

	ID3D11VertexShader* vertexShader = nullptr;

	DirectX::XMVECTOR pos;

	Object(D3DContext* d3dContext, DirectX::XMVECTOR pos, 
		UINT numVertices, T* vertices, UINT numIndices, WORD* indices, 
		ID3D11VertexShader* vs, ID3D11InputLayout* inputLayout, Material* material);
	int CreateVertexBuffer(D3DContext* d3dContext, UINT numVertices, T* vertices);
	int CreateIndexBuffer(D3DContext* d3dContext, UINT numIndices, WORD* indices);
	DirectX::XMMATRIX GetModelMatrix();
	virtual DirectX::XMVECTOR& GetPos();
	void RenderObject(D3DContext* d3dContext, float deltaTime);

	float angle;
};


// OBJECT<T> IMPL

template <typename T>
Object<T>::Object(D3DContext* d3dContext, DirectX::XMVECTOR pos, 
	UINT numVertices, T* vertices, UINT numIndices, WORD* indices,
	ID3D11VertexShader* vs, ID3D11InputLayout* inputLayout, Material* material)
	: pos(pos), material(material), vertexShader(vs), d3dInputLayout(inputLayout), numIndices(numIndices) {
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
int Object<T>::CreateIndexBuffer(D3DContext* d3dContext, UINT numIndices, WORD* indices) {
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * numIndices;
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

template <typename T>
void Object<T>::RenderObject(D3DContext* d3dContext, float deltaTime) {
	const UINT vertexStride = sizeof(T);
	const UINT offset = 0;
	
	ID3D11DeviceContext* d3dDeviceContext = d3dContext->d3dDeviceContext;

	d3dDeviceContext->IASetVertexBuffers(0, 1, &d3dVertexBuffer, &vertexStride, &offset);
	d3dDeviceContext->IASetIndexBuffer(d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dDeviceContext->VSSetShader(vertexShader, nullptr, 0);
	d3dDeviceContext->PSSetShader(material->pixelShader, nullptr, 0);
	d3dDeviceContext->IASetInputLayout(d3dInputLayout);

	if (material->shaderResourceView) {
		d3dDeviceContext->PSSetShaderResources(0, 1, &material->shaderResourceView);
		d3dDeviceContext->PSSetSamplers(0, 1, &material->samplerState);
	}

	angle += 90.f * deltaTime;

	DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMMATRIX modelMatrix = GetModelMatrix();
	DirectX::XMMATRIX modelMatrixRotated = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(angle)), modelMatrix);
	d3dDeviceContext->UpdateSubresource(d3dContext->d3dConstantBuffers[CB_Object], 0, nullptr, &modelMatrixRotated, 0, 0);

	d3dDeviceContext->DrawIndexed(numIndices, 0, 0);
}
