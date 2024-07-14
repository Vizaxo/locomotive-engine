cbuffer Sprite : register(b1) {
	float2 pos;
	float2 size;
	float3 color;
}


struct VSOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(VSOut psIn) : SV_TARGET
{
	return float4(psIn.uv, 0.0f, 1.0f);
}