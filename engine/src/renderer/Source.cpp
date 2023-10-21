#include "PCH.h"

#include "Scene.h"

DirectX::XMMATRIX Scene::localToWorld() {
	DirectX::XMVECTOR worldUp = DirectX::XMVectorSet(0,1,0,0);
	DirectX::XMVECTOR worldForward = lookDirection;
	DirectX::XMVECTOR worldRight = DirectX::XMVector3Cross(worldForward, worldUp);
	DirectX::XMVECTOR up = DirectX::XMVector3Cross(worldRight, worldForward);
	return DirectX::XMMATRIX(worldRight, up, worldForward, DirectX::XMVectorSet(0,0,0,1));
}