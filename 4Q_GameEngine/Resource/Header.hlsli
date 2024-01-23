
cbuffer ProjectionBuffer : register(b0)
{
    matrix Projection;
}

cbuffer ViewBuffer : register(b1)
{
    matrix View;
    
}
cbuffer WorldBuffer : register(b2)
{
    matrix World;
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
};