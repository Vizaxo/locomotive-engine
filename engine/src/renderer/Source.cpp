#include "PCH.h"

#include "Scene.h"

DirectX::XMVECTOR Scene::worldRight() {
	return DirectX::XMVector3Cross(lookDirection, worldUp);
}

DirectX::XMMATRIX Scene::localToWorld() {
	DirectX::XMVECTOR worldForward = lookDirection;
	DirectX::XMVECTOR up = DirectX::XMVector3Cross(worldRight(), worldForward);
	return DirectX::XMMATRIX(worldRight(), up, worldForward, DirectX::XMVectorSet(0,0,0,1));
}