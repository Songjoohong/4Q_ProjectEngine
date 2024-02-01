#include "Header.hlsli"

// Vertex Shader(VS) 프로그래밍
PS_INPUT main(STATIC_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    // 오브젝트 월드 변환
    float4 pos = float4(input.pos, 1.f);
    pos = mul(pos, World);
    pos = mul(pos, ShadowView);
    pos = mul(pos, ShadowProjection);
    output.PosProjection = pos;
    
    return output;
}