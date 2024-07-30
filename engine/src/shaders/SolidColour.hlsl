struct VSOut {
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
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

VSOut vsMain(float3 pos : POSITION, float3 normal : NORMAL) {
	VSOut ret;
	//ret.pos = float4(pos, 1.0); //mul(mvp, float4(pos, 1.0));
	ret.pos = mul(mvp, float4(pos, 1.0)); //mul(mvp, float4(pos, 1.0));
	ret.normal = float4(normal, 1.0); //TODO: transform normals

	return ret;
}

float4 psMain(in VSOut vsOut) : SV_TARGET0 {
	return vsOut.normal;
}