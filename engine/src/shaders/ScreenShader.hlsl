float4 main( float4 pos : POSITION, float2 uv : TEXCOORD, out float2 uvOut : TEXCOORD ) : SV_POSITION
{
	uvOut = uv;
	return pos;
}