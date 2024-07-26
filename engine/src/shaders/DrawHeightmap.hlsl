Texture2D<float> heightmap : register(t0);
SamplerState heightmapSampler : register(s0);

float4 main(in float4 pos : SV_POSITION, in float2 uv : TEXCOORD) : SV_TARGET
{
	return float4(0.f, heightmap.Sample(heightmapSampler, uv), 0.f, 1.0f);
}