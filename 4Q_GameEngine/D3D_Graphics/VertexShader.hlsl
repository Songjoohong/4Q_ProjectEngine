#include "Header.hlsli"

float4 main( STATIC_INPUT input ) :SV_Position
{
    return (input.pos, 1.0f);
}