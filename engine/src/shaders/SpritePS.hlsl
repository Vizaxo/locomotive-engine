cbuffer SpriteComponent : register(b1) {
	float2 pos;
	float2 size;
	float2 index;
	float2 numTiles;
}

cbuffer SpriteSheet : register(b2) {
	float2 tileSize;
	float2 sheetSize;
}

Texture2D SpriteTexture;

SamplerState PointSampler;

struct VSOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_TARGET
{
	float2 tileSizeUV = tileSize / sheetSize;
	float2 spriteSizeUV = tileSizeUV * numTiles;
	float2 spriteIndexUV = index / sheetSize;

	float2 outUV = spriteIndexUV + spriteSizeUV * uv;
	return SpriteTexture.Sample(PointSampler, outUV);
}