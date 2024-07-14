cbuffer View : register(b0) {
	float2 windowSize;
}

cbuffer Sprite : register(b1) {
	float2 pos;
	float2 size;
}

struct VSOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VSOut main( float4 vertPos : POSITION, float2 uv : TEXCOORD )
{
	float2 posOut = (((vertPos+float2(1, 1))/float2(2,2)) * size + pos) / (windowSize/2) - float2(1,1);
	VSOut ret;
	ret.pos = float4(posOut.x, posOut.y, 0.0, 1.0);
	ret.uv = uv;
	return ret;
}