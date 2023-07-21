sampler sampleDiffuse : register(ps, s0);
Texture2D<float4> texDiffuse : register(t0);

sampler sampleNormal : register(s1);
Texture2D<float4> texNormal : register(t1);

sampler sampleWorldPos : register(s2);
Texture2D<float4> texWorldPos : register(t2);

float2 windowSize : register (b0);

float4 main(float4 uv : SV_POSITION) : SV_TARGET
{
	float3 lightPos = float3(0.0f, 3.0f, 0.0f);

	float3 normal = texNormal.Sample(sampleNormal, uv.xy / windowSize.xy);
	float3 diffuse = texDiffuse.Sample(sampleDiffuse, uv.xy / windowSize.xy);
	float3 worldPos = texWorldPos.Sample(sampleWorldPos, uv.xy / windowSize.xy);

	float3 lightDir = normalize(worldPos - lightPos);
	float attenuation = dot(normal, -lightDir);
	float3 lightColour = float3(1.0f, 1.0f, 1.0f);
	float ambient = 0.5 * float3(1.0f, 0.3f, 0.8f);

	return float4((attenuation * lightColour + ambient) * diffuse, 0.0f);
}