Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txEmissive : register(t3);
Texture2D txOpacity : register(t4);
Texture2D txMetalic : register(t5);
Texture2D txRoughness : register(t6);
Texture2D txShadow : register(t7);
SamplerState samplerLinear : register(s0);
SamplerState samplerMirror : register(s1);

cbuffer ProjectionBuffer : register(b0)
{
    matrix Projection;
    matrix ShadowProjection;
}

cbuffer ViewBuffer : register(b1)
{
    matrix View;
    matrix ShadowView;
    
}
cbuffer WorldBuffer : register(b2)
{
    matrix World;
}

cbuffer LightBuffer : register(b3)
{
    float4 Direction;
}

struct STATIC_INPUT
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;    
};

struct PS_INPUT
{
    float4 PosProjection : SV_POSITION;
    float3 PosWorld : POSITION;
    float2 Texcoord : TEXCOORD0;
    float3 NorWorld : NORMAL;
    float3 TanWorld : TANGENT;
    float4 PosShadow : TEXCOORD1;
};