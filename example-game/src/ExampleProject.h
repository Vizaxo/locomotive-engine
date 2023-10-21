#pragma once

#include "PCH.h"

#include "rhi/RHI.h"
#include "renderer/Scene.h"
#include "renderer/Material.h"
#include "renderer/Mesh.h"
#include "renderer/Object.h"
#include "Application.h"
#include "input/Mouse.h"

namespace Application {

void makePlane(D3DContext* d3dContext);
void createHexMesh(D3DContext* d3dContext);
void setupLighting();
void setupCamera();

Scene scene;

struct VertexPosColor {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

struct VertexPosUV {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};

};
