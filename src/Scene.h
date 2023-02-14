#pragma once

#include "Object.h"
#include "D3DContext.h"
#include <vector>

class Scene {
public:
	std::vector<IObject*> objects;

	void RenderScene(D3DContext* d3dContext, float deltaTime);
	void Initialise(D3DContext* d3dContext);

	ID3D11VertexShader* ScreenShader = nullptr;
	ID3D11PixelShader* GBufferCompositeShader = nullptr;
	ID3D11InputLayout* GBufferCompositeInputLayout = nullptr;
	ID3D11Buffer* GBufferCompositeVertexBuffer = nullptr;
	ID3D11ShaderResourceView* GBufferDiffuseSRV = nullptr;
	ID3D11SamplerState* GBufferDiffuseSamplerState = nullptr;
};
