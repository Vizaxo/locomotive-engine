#include "DirectXTemplatePCH.h"

#include "Object.h"

#include "D3DContext.h"

using namespace DirectX;

Object::Object(D3DContext* d3dContext, DirectX::XMMATRIX modelMatrix, 
	UINT numVertices, VertexPosColor* vertices, UINT numIndices, WORD* indices,
	ID3D11VertexShader* vs, ID3D11InputLayout* inputLayout, Material* material)
	: modelMatrix(modelMatrix), material(material), vertexShader(vs), d3dInputLayout(inputLayout), numIndices(numIndices) {
	CreateVertexBuffer(d3dContext, numVertices, vertices);
	CreateIndexBuffer(d3dContext, numIndices, indices);

	angle = 0.0f;
}

int Object::CreateVertexBuffer(D3DContext* d3dContext, UINT numVertices, VertexPosColor* vertices) {
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosColor) * numVertices;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = vertices;

	HRASSERT(d3dContext->d3dDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &d3dVertexBuffer));

	return 0;
}

int Object::CreateIndexBuffer(D3DContext* d3dContext, UINT numIndices, WORD* indices) {
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

void Object::RenderObject(D3DContext* d3dContext, float deltaTime) {
	const UINT vertexStride = sizeof(VertexPosColor);
	const UINT offset = 0;
	
	ID3D11DeviceContext* d3dDeviceContext = d3dContext->d3dDeviceContext;

	d3dDeviceContext->IASetVertexBuffers(0, 1, &d3dVertexBuffer, &vertexStride, &offset);
	d3dDeviceContext->IASetIndexBuffer(d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dDeviceContext->VSSetShader(vertexShader, nullptr, 0);
	d3dDeviceContext->PSSetShader(material->pixelShader, nullptr, 0);
	d3dDeviceContext->IASetInputLayout(d3dInputLayout);

	angle += 90.f * deltaTime;

	XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);
	XMMATRIX modelMatrixRotated = XMMatrixMultiply(XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle)), modelMatrix);
	d3dDeviceContext->UpdateSubresource(d3dContext->d3dConstantBuffers[CB_Object], 0, nullptr, &modelMatrixRotated, 0, 0);

	d3dDeviceContext->DrawIndexed(numIndices, 0, 0);
}
