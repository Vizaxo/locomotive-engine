struct VSOut {
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
};

cbuffer View : register(b0) {
	float2 windowSize;
	float2 camPos;
	float4x4 mvp;
	float camZoom;
}

cbuffer SolidColour : register (b1) {
	float4 colour;
}

Texture2D tex : register(t0);
sampler samp : register(s0);

VSOut vsMain(float3 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD) {
	VSOut ret;
	//ret.pos = float4(pos, 1.0); //mul(mvp, float4(pos, 1.0));
	ret.pos = mul(mvp, float4(pos, 1.0)); //mul(mvp, float4(pos, 1.0));
	ret.normal = float4(normal, 1.0); //TODO: transform normals
	ret.uv = uv;

	return ret;
}

float4 psMain(in VSOut vsOut) : SV_TARGET0 {
	float2 uv = float2(vsOut.uv.x, -vsOut.uv.y);
	return tex.Sample(samp, uv);
}