struct PixelShaderInput {
	float4 color : COLOR;
	float4 normal : NORMAL;
	float4 worldPos : POSITION;
};

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 worldPos : SV_TARGET2;
};

PixelShaderOutput main(PixelShaderInput IN)
{
	PixelShaderOutput OUT;
	OUT.color = IN.color;
	OUT.normal = IN.normal;
	OUT.worldPos = IN.worldPos;
	return OUT;
}