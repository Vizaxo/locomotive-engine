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

struct PointLight {
	float3 pos;
	float3 colour;
	float radius;
};

struct DirectionalLight {
	float3 direction;
	float3 colour;
};

cbuffer lights : register(b1) {
	PointLight pointLights[32];
	DirectionalLight directionalLight;
	int numPointLights;
}

float3 aces_approx(float3 v)
{
    v *= 0.6f;
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((v*(a*v+b))/(v*(c*v+d)+e), 0.0f, 1.0f);
}

float3 reinhard_extended(float3 v, float whitePoint) {
	float3 numerator = v * (1.0f + (v / (whitePoint * whitePoint)));
	return numerator / (1.0f + v);
}

float4 main(float4 uv : SV_POSITION) : SV_TARGET
{
	float4 GBufferA = texDiffuse.Sample(sampleDiffuse, uv.xy / windowSize.xy);
	float3 diffuse = GBufferA.xyz;
	float specular = GBufferA.w;
	float3 worldPos = texWorldPos.Sample(sampleWorldPos, uv.xy / windowSize.xy);
	float3 normal = texNormal.Sample(sampleNormal, uv.xy / windowSize.xy);

	float3 colour = 0.0f;

	for (int i = 0; i < numPointLights; i++) {

		float3 lightPos = pointLights[i].pos;

		float3 lightVec = worldPos - lightPos;
		float3 lightDir = -normalize(lightVec);
		float lightDist = length(lightVec);
		float angleAttenuation = max(dot(normal, lightDir), 0);

		//Exponential falloff
		//TODO: make quadratic falloff
		float distanceAttenuation = max(pow(2, -(1 / (pointLights[i].radius * 0.5)) * lightDist), 0);

		colour += pointLights[i].colour * distanceAttenuation * angleAttenuation * diffuse;

		float3 viewDir = normalize(cameraPos - worldPos);
		float3 reflectDir = reflect(-lightDir, normal);
		float3 specAttenuation = pow(max(dot(viewDir, reflectDir), 0), 32);

		colour += pointLights[i].colour * specAttenuation * specular;
	}

	float3 lightDir = directionalLight.direction;
	float angleAttenuation = max(dot(normal, -lightDir), 0);

	colour += directionalLight.colour * angleAttenuation * diffuse;

	float3 viewDir = normalize(cameraPos - worldPos);
	float3 reflectDir = reflect(lightDir, normal);
	float3 specAttenuation = pow(max(dot(viewDir, reflectDir), 0), 32);

	colour += directionalLight.colour * specAttenuation * specular;


	float3 ambient = 0.1 * float3(0.7f, 0.7f, 1.0f);
	colour += ambient * diffuse;

	// naive (non-luminance) Reinhard tonemapping
	//colour = aces_approx(colour);

	float exposure = 1.0f/50.0f;
	colour *= exposure;

	colour = colour / (1 + colour);

	float gamma = 2.2f;
	return float4(pow(colour, 1.0/gamma), 0.0f);
}