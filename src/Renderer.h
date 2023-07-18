#pragma once

#include "DirectXTemplatePCH.h"

#include "D3DContext.h"
#include "Object.h"
#include "Scene.h"

enum ConstantBuffer {
	CB_Application,
	CB_Frame,
	CB_Object,
	NumConstantBuffers
};

class Renderer {
public:
	void RenderScene(D3DContext* d3dContext, Scene& scene, float deltaTime);
	void Initialise(D3DContext* d3dContext);
	void RenderObject(D3DContext* d3dContext, float deltaTime, Object& obj);
	~Renderer();

	ID3D11VertexShader* ScreenShader = nullptr;
	ID3D11PixelShader* GBufferCompositeShader = nullptr;
	ID3D11InputLayout* GBufferCompositeInputLayout = nullptr;
	ID3D11Buffer* GBufferCompositeVertexBuffer = nullptr;
	ID3D11ShaderResourceView* GBufferDiffuseSRV = nullptr;
	ID3D11SamplerState* GBufferDiffuseSamplerState = nullptr;

	ID3D11Texture2D* GBufferTextures[GBUFFER_COUNT] = {nullptr, nullptr, nullptr};
	ID3D11RenderTargetView* GBuffer[GBUFFER_COUNT] = {nullptr, nullptr, nullptr};

	ID3D11Buffer* cbWindowSize = nullptr;

	ID3D11Buffer* d3dConstantBuffers[NumConstantBuffers];

	DirectX::XMMATRIX WorldMatrix;
	DirectX::XMMATRIX ViewMatrix;
	DirectX::XMMATRIX ProjectionMatrix;
};
