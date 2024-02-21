#include"Header.hlsli"

Outline_PS_INPUT main(STATIC_INPUT input)
{
    Outline_PS_INPUT output;
    output.pos = float4(input.pos *1.003f, 1);
    output.pos = mul(output.pos, World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    return output;
}