#include "PCH.h"
#include "Object.h"
#include "Mesh.h"

Object::Object(D3DContext* d3dContext, DirectX::XMVECTOR pos, float angle, Mesh& mesh, Material* material)
	: pos(pos), angle(angle), mesh(mesh), material(material)
{}

DirectX::XMMATRIX Object::GetModelMatrix() {
	return DirectX::XMMatrixTranslationFromVector(pos);
}

DirectX::XMVECTOR& Object::GetPos() {
	return pos;
}
