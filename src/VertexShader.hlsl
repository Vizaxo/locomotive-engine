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
	float3 color : COLOR;
};

struct VertexShaderOutput {
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

VertexShaderOutput main( AppData IN) {
	VertexShaderOutput OUT;

	matrix mvp = mul(proj, mul(view, world));
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.color = float4(IN.color, 1.0f);
	
	return OUT;
}

// load shaders -- in precompiled headers