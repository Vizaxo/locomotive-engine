#pragma once

#include "PCH.h"

#include "rhi/RHI.h"
#include "renderer/Scene.h"
#include "renderer/Material.h"
#include "renderer/Mesh.h"
#include "renderer/Object.h"
#include "Application.h"
#include "input/Mouse.h"

class ExampleApplication : public Application {
	void init(D3DContext* d3dContext, PAL::WindowHandle* h);
	void tick(float deltaTime);
	void cleanup();
	Scene& getScene();

	void makePlane(D3DContext* d3dContext);
	RefPtr<Mesh> createHexMesh(D3DContext* d3dContext);
	void setupLighting();
	void setupCamera();
	void mouseButtonDown(Mouse::Button b);
	void mouseButtonUp(Mouse::Button b);

	Scene scene;

private:
	PixelShader* baseColourPixelShader = nullptr;
	VertexShader* baseColourVertexShader = nullptr;
};


struct VertexPosColor {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

struct VertexPosUV {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};
