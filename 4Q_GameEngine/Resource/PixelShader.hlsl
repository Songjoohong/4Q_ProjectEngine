#include "Header.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    // Diffuse�� DirectionalLight ����
    float3 Normal = normalize(input.NorWorld);
    float3 LightDirection = normalize(Direction.xyz);
    float NDotL = max(dot(Normal, -LightDirection), 0);
    float4 BaseColor = NDotL * txDiffuse.Sample(samplerLinear, input.Texcoord);

    //// �׸���ó�� �κ�
	// ����NDC ��ǥ�迡���� ��ǥ�� ��������� �����Ƿ� ����Ѵ�.
    float currentShadowDepth = input.PosShadow.z / input.PosShadow.w;
	// ����NDC ��ǥ�迡���� x(-1 ~ +1) , y(-1 ~ +1)  
    float2 uv = input.PosShadow.xy / input.PosShadow.w;
	// NDC��ǥ�� ��ǥ�� ���ø��ϱ����� Texture ��ǥ��� ��ȯ�Ѵ�.
    uv.y = -uv.y; // y�� �ݴ�
    uv = uv * 0.5 + 0.5; // -1 ���� 1�� 0~1�� ��ȯ
	
	// ShadowMap�� ��ϵ� Depth�������� 
	// Ŀ���Ҽ� �ִ� ������ �ƴϸ� ó����������
    float3 directionLighting = 0.0f;
    if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0)
    {
        float sampleShadowDepth = txShadow.Sample(samplerLinear, uv).r;
		// currentShadowDepth�� ũ�� �� ���ʿ� �����Ƿ� �������� ���ܵȴ�.
        if (currentShadowDepth > sampleShadowDepth + 0.001)
        {
            directionLighting = 0.0f;
        }
    }
    

    float3 final = directionLighting + BaseColor;

    return float4(final, 1.0f);



}