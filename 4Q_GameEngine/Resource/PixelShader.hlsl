#include "Header.hlsli"


float4 main(PS_INPUT input) : SV_TARGET
{
    float3 Normal = normalize(input.NorWorld);
    float3 LightDirection = -normalize(Direction.xyz);
    float NDotL = max(dot(Normal, LightDirection), 0);

    float4 BaseColor = txDiffuse.Sample(samplerLinear, input.Texcoord);
    float3 directionLighting = NDotL * BaseColor;

    // 포인트 라이트
    float3 PointLight = 0;

    float3 PointLightVector = normalize(input.PosWorld - PointLightPos);
    float PointLightDiffuse = max(dot(-PointLightVector, Normal), 0);
    float PointLightDistance = distance(PointLightPos, input.PosWorld);
    float3 PhongD = PointLightDiffuse * (PointLightColor.rgb * BaseColor.rgb);
    float3 PhongS = float3(0.f, 0.f, 0.f);
    float Attenuation = 0.f;

    if (PointLightDiffuse > 0.f)
    {
        float3 ViewVector = normalize(input.PosWorld - CameraPos);
        float3 ReflectionVector = normalize(reflect(PointLightVector.xyz, Normal));
        float Specular = max(dot(ReflectionVector, -ViewVector), 0);
        Specular = pow(Specular, 30.f);
        PhongS = Specular * (PointLightColor.rgb * float3(0.7f, 0.7f, 0.7f));
        if (PointLightDistance <= Radius)
        {
            Attenuation = 1.f / (1.f + (LinearTerm * PointLightDistance) +
            (QuadraticTerm * (PointLightDistance * PointLightDistance)));
        }
    }

    // 그림자
    float currentShadowDepth = input.PosShadow.z / input.PosShadow.w;
    float2 uv = input.PosShadow.xy / input.PosShadow.w;
    uv.y = -uv.y;
    uv = uv * 0.5 + 0.5;

    if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0)
    {
        float sampleShadowDepth = txShadow.Sample(samplerLinear, uv).r;
		
        if (currentShadowDepth > sampleShadowDepth + 0.001)
        {
            directionLighting = 0.0f;
        }
    }

    // 앰비언트
    float4 ambient = BaseColor * 0.3;

    // 최종 색상
    //float3 final = directionLighting + ambient;
    float3 final = Attenuation * Intensity * (PhongD + PhongS);
    final += ambient + directionLighting;

    return float4(final, 1.0f);

}