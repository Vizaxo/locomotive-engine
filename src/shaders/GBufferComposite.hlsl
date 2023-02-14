sampler sampleDiffuse : register(ps, s0);
Texture2D<float4> texDiffuse : register(t0);

sampler sampleNormal : register(s1);
Texture2D<float4> texNormal : register(t1);

sampler sampleZ : register(s2);
Texture2D<float4> texZ : register(t2);

float4 main( float4 pos : SV_POSITION ) : SV_TARGET
{
	return texDiffuse.Sample(sampleDiffuse, pos.xy/1000.0);
}