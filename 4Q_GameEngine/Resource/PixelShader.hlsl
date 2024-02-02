#include "Header.hlsli"


float4 main(PS_INPUT input) : SV_TARGET
{

    float3 Normal = normalize(input.NorWorld);
    float3 LightDirection = -normalize(Direction.xyz);
    float NDotL = max(dot(Normal, LightDirection), 0);

    float4 BaseColor = txDiffuse.Sample(samplerLinear, input.Texcoord);
    float3 directionLighting = NDotL * BaseColor;
    float3 pointlight = 0;
    
    float3 LightVector = LightPos - input.PosWorld;
    float4 light = { 1.f, 0.f, 0.f, 1.f};
    float len = length(LightVector);
    float att = 1;
    if (len < Radius)
    {
        float3 NL = LightVector / len;
        att = 2000 / (len * len);
        pointlight = BaseColor * att;
    }

    float currentShadowDepth = input.PosShadow.z / input.PosShadow.w;

    float2 uv = input.PosShadow.xy / input.PosShadow.w;

    uv.y = -uv.y; 
    uv = uv * 0.5 + 0.5; 

    
    if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0)
    {
        float sampleShadowDepth = txShadow.Sample(samplerLinear, uv).r;
		
        if (currentShadowDepth > sampleShadowDepth + 0.001)
        {
            directionLighting = 0.0f;
        }
    }

    float4 ambient = BaseColor * 0.3;
    float3 final = directionLighting + ambient + pointlight;

    return float4(final, 1.0f);

}