#include "Header.hlsli"

float4 main(ENVIRONMENT_PS_INPUT input) : SV_Target
{
    float4 color = txEnvironment.Sample(samplerLinear, input.TexCoords);
    //color.rgb = pow(color.rgb, 2.2);
    
    color.rgb = pow(color.rgb, 1 / 2.2);

    return color;
}