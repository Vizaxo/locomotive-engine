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
	Renderer(D3DContext* d3dContext);
	~Renderer();
	void RenderScene(D3DContext* d3dContext, Scene& scene, float deltaTime);
	void RenderObject(D3DContext* d3dContext, float deltaTime, Object& obj);

	VertexShader screenShader;
	PixelShader GBufferCompositeShader;
	ID3D11InputLayout* GBufferCompositeInputLayout = nullptr;
	VertexBuffer* GBufferCompositeVertexBuffer;
	ID3D11ShaderResourceView* GBufferDiffuseSRV = nullptr;
	ID3D11SamplerState* GBufferDiffuseSamplerState = nullptr;

	ID3D11Texture2D* GBufferTextures[GBUFFER_COUNT] = {nullptr, nullptr, nullptr};
	ID3D11RenderTargetView* GBuffer[GBUFFER_COUNT] = {nullptr, nullptr, nullptr};

	ID3D11Buffer* cbWindowSize = nullptr;

	ID3D11Buffer* d3dConstantBuffers[NumConstantBuffers];

	DirectX::XMMATRIX WorldMatrix      = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ViewMatrix       = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ProjectionMatrix = DirectX::XMMatrixIdentity();
};
