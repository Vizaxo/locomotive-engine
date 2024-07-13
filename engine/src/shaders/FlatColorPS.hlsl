cbuffer Zero : register(b0) {
	float3 color;
}

float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
	return float4(color, 1.0f);
}