#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Material.h"
#include "Mesh.h"

class Renderer;

class Object {
public:
	DirectX::XMVECTOR& GetPos();
	DirectX::XMMATRIX GetModelMatrix();
	Object(D3DContext* d3dContext, DirectX::XMVECTOR pos, float angle, Mesh& mesh, Material* material);

	Material* material;
	Mesh& mesh;
	DirectX::XMVECTOR pos;
	float angle;
	UINT vertexStride;
};

