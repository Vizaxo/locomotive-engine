cbuffer Sprite : register(b1) {
	float2 pos;
	float2 size;
}

Texture2D SpriteTexture;

SamplerState PointSampler {
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VSOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_TARGET
{
	return SpriteTexture.Sample(PointSampler, uv);
}