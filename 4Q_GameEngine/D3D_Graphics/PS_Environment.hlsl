#include "Header.hlsli"

float4 main(ENVIRONMENT_PS_INPUT input) : SV_Target
{
    return txEnvironment.Sample(samplerLinear, input.TexCoords);
}