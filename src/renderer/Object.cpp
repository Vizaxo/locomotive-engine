#include "Object.h"
#include "Mesh.h"

#include "D3DContext.h"

Object::Object(D3DContext* d3dContext, DirectX::XMVECTOR pos, float angle, Mesh& mesh, Material* material)
	: pos(pos), angle(angle), mesh(mesh), material(material)
{}

DirectX::XMMATRIX Object::GetModelMatrix() {
	return DirectX::XMMatrixTranslationFromVector(pos);
}

DirectX::XMVECTOR& Object::GetPos() {
	return pos;
}
