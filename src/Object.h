#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"

class Object {
public:
	ID3D11Buffer* d3dVertexBuffer = nullptr;
	ID3D11Buffer* d3dIndexBuffer = nullptr;

	UINT numIndices;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	DirectX::XMMATRIX modelMatrix;

	Object(D3DContext* d3dContext, DirectX::XMMATRIX modelMatrix, UINT numVertices, VertexPosColor* vertices, UINT numIndices, WORD* indices, ID3D11PixelShader* ps, ID3D11VertexShader* vs);
	int CreateVertexBuffer(D3DContext* d3dContext, UINT numVertices, VertexPosColor* vertices);
	int CreateIndexBuffer(D3DContext* d3dContext, UINT numIndices, WORD* indices);
	void RenderObject(D3DContext* d3dContext, float deltaTime);

	float angle;
};
