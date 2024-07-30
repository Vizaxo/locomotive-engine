struct VSOut {
	float4 pos : SV_POSITION;
};

cbuffer View : register(b0) {
	float2 windowSize;
	float2 camPos;
	float camZoom;
}

cbuffer SolidColour : register (b1) {
	float4 colour;
}

VSOut vsMain(float4 pos : POSITION) {
	VSOut ret;
	ret.pos = pos;

	return ret;
}

float4 psMain(in VSOut vsOut) : SV_TARGET0 {
	return colour;
}