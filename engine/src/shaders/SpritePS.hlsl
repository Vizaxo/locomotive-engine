struct SpriteComponent {
	float2 pos;
	float2 size;
	float2 index;
	float2 numTiles;
	float2x4 PAD;
	uint entityID;
	float PAD2;
};

StructuredBuffer<SpriteComponent> spriteData : register(t1);

cbuffer SpriteSheet : register(b2) {
	float2 tileSize;
	float2 sheetSize;
}

Texture2D SpriteTexture;

SamplerState PointSampler;

struct VSOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	uint instanceID : SV_InstanceID;
};

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD, uint instanceID : SV_InstanceID) : SV_TARGET
{
	SpriteComponent sprite = spriteData[instanceID];

	float2 tileSizeUV = tileSize / sheetSize;
	float2 spriteSizeUV = tileSizeUV * sprite.numTiles;
	float2 spriteIndexUV = sprite.index / sheetSize;

	float2 outUV = spriteIndexUV + spriteSizeUV * uv;
	return SpriteTexture.Sample(PointSampler, outUV);
}