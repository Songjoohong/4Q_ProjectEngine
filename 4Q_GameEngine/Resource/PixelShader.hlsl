#include "Header.hlsli"


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 BaseColor = txDiffuse.Sample(samplerState, input.Texcoord);
    float3 LightVector = LightPos - input.PosWorld;
    float4 light=1.f;
    float len = length(LightVector);
    float att = 1;
    if(len<Radius)
    {
        float3 NL = LightVector / len;
        att = 2000 / (len * len);
        
    }
    
    return BaseColor * att;

}