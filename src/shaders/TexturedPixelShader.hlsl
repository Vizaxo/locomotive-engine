struct PixelShaderInput {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

sampler ss : register(s0);
Texture2D<float4> tex : register(t0);

float4 main(PixelShaderInput IN) : SV_TARGET
{
	return tex.Sample(ss, IN.uv);
}