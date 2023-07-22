sampler sampleDiffuse : register(ps, s0);
Texture2D<float4> texDiffuse : register(t0);

sampler sampleNormal : register(s1);
Texture2D<float4> texNormal : register(t1);

sampler sampleWorldPos : register(s2);
Texture2D<float4> texWorldPos : register(t2);

cbuffer frameData : register (b0) {
	float2 windowSize;
	float3 cameraPos;
}

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
	float4 GBufferA = texDiffuse.Sample(sampleDiffuse, uv.xy / windowSize.xy);
	float3 diffuse = GBufferA.xyz;
	float specular = GBufferA.w;
	float3 worldPos = texWorldPos.Sample(sampleWorldPos, uv.xy / windowSize.xy);
	float3 normal = texNormal.Sample(sampleNormal, uv.xy / windowSize.xy);

	float3 colour = 0.0f;

	for (int i = 0; i < numLights; i++) {

		float3 lightPos = lights[i].pos;

		float3 lightVec = worldPos - lightPos;
		float3 lightDir = -normalize(lightVec);
		float lightDist = length(lightVec);
		float angleAttenuation = dot(normal, lightDir);

		//Exponential falloff
		float distanceAttenuation = pow(2, -(1 / (lights[i].radius * 0.5)) * lightDist);

		colour += lights[i].colour * distanceAttenuation * angleAttenuation * diffuse;

		float3 viewDir = normalize(cameraPos - worldPos);
		float3 reflectDir = reflect(-lightDir, normal);
		float3 specAttenuation = pow(max(dot(viewDir, reflectDir), 0), 32);

		colour += lights[i].colour * specAttenuation * specular;
	}

	float3 ambient = 0.1 * float3(0.7f, 0.7f, 1.0f);
	colour += ambient;

	return float4(colour, 0.0f);
}