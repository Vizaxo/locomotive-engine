Texture2D<float> heightmap : register(t0);
SamplerState heightmapSampler : register(s0);

float4 main(in float4 pos : SV_POSITION, in float2 uv : TEXCOORD) : SV_TARGET
{
	float h = heightmap.Sample(heightmapSampler, uv) / 100.f + 0.5f;
	return float4(0.f, h, 0.f, h);
}