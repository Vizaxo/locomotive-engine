cbuffer View : register(b0) {
	float2 windowSize;
	float2 camPos;
	float camZoom;
}

struct VSOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	uint instanceID : SV_InstanceID;
};

struct SpriteComponent {
	float2 pos;
	float2 size;
	float2 index;
	float2 numTiles;
	uint entityID;
	float PAD2;
};

StructuredBuffer<SpriteComponent> spriteData : register(t1);

VSOut main( float4 vertPos : POSITION, float2 uv : TEXCOORD, uint instanceID : SV_InstanceID )
{
	//TODO: windowsize should be /2. Add explicit scaling option.
	SpriteComponent sprite = spriteData[instanceID];
	float2 posOut = ((((vertPos+float2(1, 1))/float2(2,2)) * sprite.size + sprite.pos - camPos) / ((windowSize)) - float2(1,1)) * camZoom;
	VSOut ret;
	ret.pos = float4(posOut.x, posOut.y, 0.0, 1.0);
	ret.uv = uv;
	ret.instanceID = instanceID;
	return ret;
}