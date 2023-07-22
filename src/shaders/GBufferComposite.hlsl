sampler sampleDiffuse : register(ps, s0);
Texture2D<float4> texDiffuse : register(t0);

sampler sampleNormal : register(s1);
Texture2D<float4> texNormal : register(t1);

sampler sampleWorldPos : register(s2);
Texture2D<float4> texWorldPos : register(t2);

float2 windowSize : register (b0);

struct Light {
	float3 pos;
	float3 colour;
	float radius;
};

cbuffer lights : register(b1) {
	Light lights[32];
	int numLights;
}

float4 main(float4 uv : SV_POSITION) : SV_TARGET
{
	float3 normal = texNormal.Sample(sampleNormal, uv.xy / windowSize.xy);
	float3 diffuse = texDiffuse.Sample(sampleDiffuse, uv.xy / windowSize.xy);
	float3 worldPos = texWorldPos.Sample(sampleWorldPos, uv.xy / windowSize.xy);

	float3 colour = 0.0f;

	for (int i = 0; i < numLights; i++) {

		float3 lightPos = lights[i].pos;

		float3 lightVec = worldPos - lightPos;
		float3 lightDir = -normalize(lightVec);
		float lightDist = length(lightVec);
		float angleAttenuation = dot(normal, lightDir);

		//Exponential falloff
		float distanceAttenuation = pow(2, -(1 / (lights[i].radius * 0.5)) * lightDist);

		colour += lights[i].colour * distanceAttenuation * angleAttenuation;
	}

	float3 ambient = 0.1 * float3(0.7f, 0.7f, 1.0f);
	colour += ambient;

	return float4(colour * diffuse, 0.0f);
}