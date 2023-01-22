#pragma once

#include "D3DContext.h"
#include "Scene.h"

Scene buildExampleScene(D3DContext* d3dContext);
void cleanupExampleScene();

struct VertexPosColor {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

struct VertexPosUV {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};
