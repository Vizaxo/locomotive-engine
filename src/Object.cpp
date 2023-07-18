#include "DirectXTemplatePCH.h"

#include "Object.h"

#include "D3DContext.h"

IObject::IObject(UINT numIndices, ID3D11InputLayout* inputLayout, ID3D11VertexShader* vs, Material* material, DirectX::XMVECTOR pos, float angle)
	: numIndices(numIndices), d3dInputLayout(inputLayout), vertexShader(vs), material(material), pos(pos), angle(angle)
{}

