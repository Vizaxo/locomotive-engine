struct VSOut {
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};

cbuffer View : register(b0) {
	float2 windowSize;
	float2 camPos;
	float camZoom;
}

cbuffer SolidColour : register (b1) {
	float4 colour;
}

VSOut vsMain(float4 pos : POSITION, float4 normal : NORMAL) {
	VSOut ret;
	ret.pos = pos;
	ret.normal = normal;

	return ret;
}

float4 psMain(in VSOut vsOut) : SV_TARGET0 {
	return vsOut.normal;
}