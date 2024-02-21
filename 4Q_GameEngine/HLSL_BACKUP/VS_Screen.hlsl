#include "Header.hlsli"

PS_INPUT main( STATIC_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    
    output.PosProjection = float4(input.pos, 1);
    output.Texcoord = input.tex;
    
    return output;
}