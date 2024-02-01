#include "Header.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    // Diffuse에 DirectionalLight 적용
    float3 Normal = normalize(input.NorWorld);
    float3 LightDirection = normalize(Direction.xyz);
    float NDotL = max(dot(Normal, -LightDirection), 0);
    float4 BaseColor = NDotL * txDiffuse.Sample(samplerLinear, input.Texcoord);

    //// 그림자처리 부분
	// 광원NDC 좌표계에서의 좌표는 계산해주지 않으므로 계산한다.
    float currentShadowDepth = input.PosShadow.z / input.PosShadow.w;
	// 광원NDC 좌표계에서의 x(-1 ~ +1) , y(-1 ~ +1)  
    float2 uv = input.PosShadow.xy / input.PosShadow.w;
	// NDC좌표계 좌표로 샘플링하기위해 Texture 좌표계로 변환한다.
    uv.y = -uv.y; // y는 반대
    uv = uv * 0.5 + 0.5; // -1 에서 1을 0~1로 변환
	
	// ShadowMap에 기록된 Depth가져오기 
	// 커버할수 있는 영역이 아니면 처리하지않음
    float3 directionLighting = 0.0f;
    if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0)
    {
        float sampleShadowDepth = txShadow.Sample(samplerLinear, uv).r;
		// currentShadowDepth가 크면 더 뒤쪽에 있으므로 직접광이 차단된다.
        if (currentShadowDepth > sampleShadowDepth + 0.001)
        {
            directionLighting = 0.0f;
        }
    }
    
    float3 final = directionLighting + BaseColor;

    return float4(final, 1.0f);
}