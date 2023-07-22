cbuffer PerApplication : register (b0) {
	matrix proj;
}

cbuffer PerFrame : register (b1) {
	matrix view;
}

cbuffer PerObject : register (b2) {
	matrix world;
}

struct AppData {
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexShaderOutput {
	float2 uv : TEXCOORD;
	float4 normal : NORMAL;
	float4 worldPos : POSITION;
	float4 position : SV_POSITION;
};

VertexShaderOutput main( AppData IN) {
	VertexShaderOutput OUT;

	matrix mvp = mul(proj, mul(view, world));
	matrix rotate = world;
	rotate._m03 = 0.0f;
	rotate._m13 = 0.0f;
	rotate._m23 = 0.0f;

	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.uv = IN.uv;
	OUT.normal = mul(rotate, float4(IN.normal, 1.0f));
	OUT.worldPos = mul(world, float4(IN.position, 1.0f));
	return OUT;
}

// load shaders -- in precompiled headers