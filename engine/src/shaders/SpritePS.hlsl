cbuffer Sprite : register(b1) {
	float2 pos;
	float2 size;
	float3 color;
}

float4 main() : SV_TARGET
{
	return float4(color, 1.0f);
}