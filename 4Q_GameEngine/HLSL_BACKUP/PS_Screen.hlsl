#include "Header.hlsli"

Texture2D txOutline : register(t13);
Texture2D txOrigin : register(t14);

float filter[5] = { 1/25,2/25,5/25,2/25,1/25 };
float filter2[5] = { -2, -1, 0, 1, 2 };

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = 0.f;
    
    float4 x = txOrigin.Sample(samplerLinear, input.Texcoord);
    float4 y = txOutline.Sample(samplerLinear, input.Texcoord);
    if(x.r>=1&&y.r<1)
    {
        color = float4(1, 1, 1, 1);
        for (int i = 0; i < 5;i++)
        {
            float4 final = txRenderMap.Sample(samplerLinear, input.Texcoord.x + (filter2[i] / 1920)) * filter[i];
            color += final;
        }
        for (int i = 0; i < 5; i++)
        {
            float4 final = txRenderMap.Sample(samplerLinear, input.Texcoord.y + (filter2[i] / 1080))*filter[i];
            color += final;
        }
    }
    color += txRenderMap.Sample(samplerLinear, input.Texcoord);
	return color;
}