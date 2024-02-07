#include "Header.hlsli"

ENVIRONMENT_PS_INPUT main(STATIC_INPUT input)
{
    ENVIRONMENT_PS_INPUT output = (ENVIRONMENT_PS_INPUT) 0;
    float4 pos = mul(float4(input.pos, 1.0), World);
    output.TexCoords = pos.xyz;
    float3 pos3 = mul(pos.xyz, (float3x3) View);
    pos = mul(float4(pos3, 1.0f), Projection);
    output.posProjection = pos;
	return output;
}