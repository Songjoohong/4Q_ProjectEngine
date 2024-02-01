#include "Header.hlsli"

// Vertex Shader(VS) ���α׷���
PS_INPUT main(STATIC_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    // ������Ʈ ���� ��ȯ
    float4 pos = float4(input.pos, 1.f);
    pos = mul(pos, World);
    pos = mul(pos, ShadowView);
    pos = mul(pos, ShadowProjection);
    output.PosProjection = pos;
    
    return output;
}