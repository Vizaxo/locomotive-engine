#pragma once

#include "PCH.h"

#include "rhi/RHI.h"
#include "Scene.h"

enum ConstantBuffer {
	CB_Application,
	CB_Frame,
	CB_Object,
	NumConstantBuffers
};

struct alignas(16) FrameData {
	union {
		DirectX::XMFLOAT2 windowSize;
		float _pad[4];
	};
	DirectX::XMVECTOR cameraPos;
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
	ID3D11ShaderResourceView* GBufferNormalSRV = nullptr;
	ID3D11SamplerState* GBufferNormalSamplerState = nullptr;
	ID3D11ShaderResourceView* GBufferWorldPosSRV = nullptr;
	ID3D11SamplerState* GBufferWorldPosSamplerState = nullptr;

	ID3D11Texture2D* GBufferTextures[GBUFFER_COUNT] = {nullptr, nullptr, nullptr};
	ID3D11RenderTargetView* GBuffer[GBUFFER_COUNT] = {nullptr, nullptr, nullptr};

	ID3D11Buffer* cbFrameData = nullptr;
	ID3D11Buffer* cbLights = nullptr;

	ID3D11Buffer* d3dConstantBuffers[NumConstantBuffers];

	DirectX::XMMATRIX WorldMatrix      = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ViewMatrix       = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX ProjectionMatrix = DirectX::XMMatrixIdentity();
};
