Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txEmissive : register(t3);
Texture2D txOpacity : register(t4);
Texture2D txMetalic : register(t5);
Texture2D txRoughness : register(t6);
SamplerState samplerState : register(s0);

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
cbuffer PointLight : register(b3)
{
    float3 LightPos;
    float Radius;
    float3 LightColor;
    float pad;
    float3 CameraPos;
    float pad2;
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