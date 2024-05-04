#pragma once

#include "PCH.h"

#include "resource-manager/ResourceManager.h"
#include "rhi/RHI.h"
#include "Material.h"
#include "Mesh.h"

class Renderer;

class Object {
public:
	DirectX::XMVECTOR& GetPos();
	DirectX::XMMATRIX GetModelMatrix();
	Object(D3DContext* d3dContext, DirectX::XMVECTOR pos, float angle, RefPtr<Mesh> mesh, RefPtr<Material> material);

	RefPtr<Material> material;
	RefPtr<Mesh> mesh;
	DirectX::XMVECTOR pos;
	float angle;
	UINT vertexStride;
};

extern ResourceManager<Object> objectManager;
