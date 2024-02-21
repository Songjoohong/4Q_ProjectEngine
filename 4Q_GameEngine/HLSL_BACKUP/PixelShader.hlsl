#include "Header.hlsli"

static const float Epsilon = 0.00001;
static const float PI= 3.141592;

static const float3 Fdielectric = 0.04;

float NDF(float NdotH,float roughness) 
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float denominator = (NdotH * NdotH) * (alphaSq - 1.0f) + 1.0f;
    return alphaSq / (PI * denominator * denominator);
}

float3 FresnelSchlick(float3 F0, float HdotV)   
{

    return F0 + (1.0f - F0) * pow(1.0f - HdotV, 5.0f);
}

float GSub(float cosTheta,float k)
{
    return cosTheta / (cosTheta * (1.0f - k) + k);
}
 
float GGX(float NdoL, float NdotV, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    return GSub(NdoL, k) * GSub(NdotV, k);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 BaseColor = txDiffuse.Sample(samplerLinear, input.Texcoord);
    float3 Normal = normalize(input.NorWorld);
    float3 Tangent = normalize(input.TanWorld);
    float3 BiTangent = cross(input.TanWorld, input.NorWorld);
    float3 LightDirection = -normalize(Direction.xyz);
    float3 ViewVector = normalize(CameraPos - input.PosWorld);
    float3 HalfVector = normalize(LightDirection + ViewVector);
    
    float3 EmissiveLighting = txEmissive.Sample(samplerLinear, input.Texcoord);
    
    float AmbientOcclusion = txMetalic.Sample(samplerLinear, input.Texcoord).r;
    
    float3 LightColor = 1.0f;
    //Normal Tangent space 
    float3 NormalTangent = txNormal.Sample(samplerLinear, input.Texcoord).rgb*2-1;
    float3x3 WorldTransform = float3x3(Tangent, BiTangent, Normal);
    Normal = mul(NormalTangent, WorldTransform);
    Normal = normalize(Normal);
    
    //dot production
  
    float NDotL = max(0, dot(Normal, LightDirection));
   
    float NDotH = max(0, dot(Normal, HalfVector));
  
    float NDotV = max(0, dot(Normal, ViewVector));
    
    //Gamma correction
    BaseColor.rgb = pow(BaseColor.rgb, 2.2);
    
    
    float Metalness = 1.0f;
    Metalness = txMetalic.Sample(samplerLinear, input.Texcoord).r;
    
    float Roughness = 0.0f;
    Roughness = txRoughness.Sample(samplerLinear, input.Texcoord).r;
    
    float3 Emissive = 0.0f;
    //Emissive = txEmissive.Sample(samplerLinear, input.Texcoord).rgb;
    
    float Opacity = 1.0f;
    Opacity = txOpacity.Sample(samplerLinear, input.Texcoord).a;
    
    float3 F0 = lerp(Fdielectric, BaseColor.rgb, Metalness);
    float3 F = FresnelSchlick(F0, max(0, dot(HalfVector, ViewVector)));
    float D = NDF(NDotH, Roughness);
    float G = GGX(NDotL, NDotV, Roughness);
    
    float3 DirectionLighting = 0.0f;
    
    float3 Kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), Metalness);
    
    float3 DiffuseBRDF = Kd * BaseColor.rgb;
    
    float3 SpecularBRDF = (F * D * G) / max(Epsilon, 4.0 * NDotL * NDotV);
    
    DirectionLighting += (DiffuseBRDF + SpecularBRDF) * LightColor * NDotL; 

    // 포인트 라이트
    float3 PointLighting = 0.0;
    for (int i = 0; i < PointLightCount; i++)
    {
        float3 ObjectToPointLight = pointLights[i].PointLightPos - input.PosWorld;
        float PointLightDistance = distance(pointLights[i].PointLightPos, input.PosWorld);
        ObjectToPointLight = normalize(ObjectToPointLight);
        float NDotPL = max(0, dot(Normal, ObjectToPointLight));
        float3 HalfVector = normalize(ObjectToPointLight + ViewVector);

        if (PointLightDistance < pointLights[i].Radius)
        {
            F = FresnelSchlick(F0, max(0, dot(HalfVector, ViewVector)));
            D = NDF(max(0, dot(Normal, HalfVector)), Roughness);
            G = GGX(NDotPL, NDotV, Roughness);

            Kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), Metalness);
            DiffuseBRDF = Kd * BaseColor.rgb;
            SpecularBRDF = (F * D * G) / max(Epsilon, 4.0 * dot(Normal, ObjectToPointLight) * NDotV);

            float Attenuation = 1.f / (1.0f + (LinearTerm * PointLightDistance) + (QuadraticTerm * (PointLightDistance * PointLightDistance)));
            PointLighting += (DiffuseBRDF + SpecularBRDF) * pointLights[i].PointLightColor * (Attenuation * pointLights[i].Intensity) * NDotPL;
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
            DirectionLighting = 0.0f;
        }
    }

    //IBL
    float3 AmbientLighting = 0.0f;
    
    {
        float3 ReflectionVector = 2.0 * NDotV * Normal - ViewVector;
        float3 Irradiance = txIBL_Diffuse.Sample(samplerLinear, Normal).rgb;
        
        float3 F = FresnelSchlick(F0, NDotV);
        
        float3 Kd = lerp(1.0 - F, 0.0, Metalness);
        
        float3 DiffuseIBL = Kd * BaseColor.rgb * Irradiance;
        
        uint Width,Height,SpecularTextureLevels;
        
        txIBL_Specular.GetDimensions(0, Width, Height, SpecularTextureLevels);
        
        float3 SpecularIrradiance = txIBL_Specular.SampleLevel(samplerLinear, ReflectionVector, Roughness*SpecularTextureLevels).rgb;

        float2 SpecularBRDF = txIBL_SpecularBRDF_LUT.Sample(samplerClamp, float2(NDotV, Roughness)).rg;
        
        float3 SpecularIBL = (F0 * SpecularBRDF.x + SpecularBRDF.y) * SpecularIrradiance;
        
        AmbientLighting = (DiffuseIBL + SpecularIBL) * 0.2;

    }
    EmissiveLighting = 0;
    float3 final = DirectionLighting + AmbientLighting + EmissiveLighting + PointLighting; //saturate(directionLighting + BaseColor);

     final.rgb = pow(final.rgb, 1 / 2.2);

    return float4(final, Opacity);


}