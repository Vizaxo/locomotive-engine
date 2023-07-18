#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Object.h"
#include "Scene.h"

class Renderer {
public:
	void RenderScene(D3DContext* d3dContext, Scene& scene, float deltaTime);
	template <typename T> void RenderObject(D3DContext* d3dContext, float deltaTime, Object<T> object);
	void Initialise(D3DContext* d3dContext);

private:
	ID3D11VertexShader* ScreenShader = nullptr;
	ID3D11PixelShader* GBufferCompositeShader = nullptr;
	ID3D11InputLayout* GBufferCompositeInputLayout = nullptr;
	ID3D11Buffer* GBufferCompositeVertexBuffer = nullptr;
	ID3D11ShaderResourceView* GBufferDiffuseSRV = nullptr;
	ID3D11SamplerState* GBufferDiffuseSamplerState = nullptr;

	ID3D11Buffer* cbWindowSize = nullptr;
};
