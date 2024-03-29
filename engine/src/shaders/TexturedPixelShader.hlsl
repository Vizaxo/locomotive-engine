struct PixelShaderInput {
	float2 uv : TEXCOORD;
	float4 normal : NORMAL;
	float4 worldPos : POSITION;
};

sampler ss : register(s0);
Texture2D<float4> tex : register(t0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 worldPos : SV_TARGET2;
};

PixelShaderOutput main(PixelShaderInput IN)
{
	PixelShaderOutput OUT;
	float gamma = 2.2;
	OUT.color = float4(pow(tex.Sample(ss, IN.uv).xyz, gamma), 0.1f);
	OUT.normal = IN.normal;
	OUT.worldPos = IN.worldPos;

	return OUT;
}