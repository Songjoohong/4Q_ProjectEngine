#include "Header.hlsli"

static const float Epsilon = 0.00001;
static const float PI = 3.141592;

static const float3 Fdielectric = 0.04;

float NDF(float NdotH, float roughness)
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

float GSub(float cosTheta, float k)
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
    float4 BaseColor = 1.0f; //txDiffuse.Sample(samplerLinear, input.Texcoord);
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
    //float3 NormalTangent = txNormal.Sample(samplerLinear, input.Texcoord).rgb;
    //float3x3 WorldTransform = float3x3(Tangent, BiTangent, Normal);
    //Normal = mul(NormalTangent, WorldTransform);
    //Normal = normalize(Normal);
    
    //dot production
  
    float NDotL = max(0, dot(Normal, LightDirection));
   
    float NDotH = max(0, dot(Normal, HalfVector));
  
    float NDotV = max(0, dot(Normal, ViewVector));
    
    //Gamma correction
    BaseColor.rgb = pow(BaseColor.rgb, 2.2);
    
    
    float Metalness = 1.0f;
    Metalness = SphereMetalic;
    
    float Roughness = 0.0f;
    Roughness = SphereRoughness;
    
    
    
    
    float3 F0 = lerp(Fdielectric, BaseColor.rgb, Metalness);
    float3 F = FresnelSchlick(F0, max(0, dot(HalfVector, ViewVector)));
    float D = NDF(NDotH, Roughness);
    float G = GGX(NDotL, NDotV, Roughness);
    
    float3 DirectionLighting = 0.0f;
    
    float3 Kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), Metalness);
    
    float3 DiffuseBRDF = Kd * BaseColor.rgb;
    
    float3 SpecularBRDF = (F * D * G) / max(Epsilon, 4.0 * NDotL * NDotV);
    
    DirectionLighting += (DiffuseBRDF + SpecularBRDF) * LightColor * NDotL;
    
    //// ï¿½×¸ï¿½ï¿½ï¿½Ã³ï¿½ï¿½ ï¿½Îºï¿½
	// ï¿½ï¿½ï¿½ï¿½NDC ï¿½ï¿½Ç¥ï¿½è¿¡ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ç¥ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿ ï¿½ï¿½ï¿½ï¿½ï¿½Ç·ï¿½ ï¿½ï¿½ï¿½ï¿½Ñ´ï¿.

    // Æ÷ÀÎÆ® ¶óÀÌÆ®
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

    // ±×¸²ÀÚ
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
    //DirectionLighting += (DiffuseBRDF + SpecularBRDF) * LightColor * NDotL;
    //float3 PointLighting = 0.0;
    
    //float3 LightVector = PointLightPos - input.PosWorld;
    //float4 light=1.f;
    //float len = length(LightVector);
    //float att = 1;
    //if (len < Radius)
    //{
    //    float3 NL = LightVector / len;
    //    F = FresnelSchlick(F0, max(0, dot(NL + ViewVector, ViewVector)));
    //    D = NDF(max(0, dot(Normal, NL + ViewVector)), Roughness);
    //    G = GGX(max(0, dot(Normal, NL)), NDotV, Roughness);
        
    //    Kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), Metalness);
    //    DiffuseBRDF = Kd * BaseColor.rgb;
        
    //    SpecularBRDF = (F * D * G) / max(Epsilon, 4.0 * dot(Normal, NL) * NDotV);
    //    float pow = 2000 / (len * len);
    //    PointLighting += (DiffuseBRDF + SpecularBRDF) * pow;
    //}
    
    //IBL
    float3 AmbientLighting = 0.0f;
    if(useIBL)
    {
        float3 ReflectionVector = 2.0 * NDotV * Normal - ViewVector;
        float3 Irradiance = txIBL_Diffuse.Sample(samplerLinear, Normal).rgb;
        
        float3 F = FresnelSchlick(F0, NDotV);
        
        float3 Kd = lerp(1.0 - F, 0.0, Metalness);
        
        float3 DiffuseIBL = Kd * BaseColor.rgb * Irradiance;
        
        int Width, Height, SpecularTextureLevels;
        
        txIBL_Specular.GetDimensions(0, Width, Height, SpecularTextureLevels);
        
        float3 SpecularIrradiance = txIBL_Specular.SampleLevel(samplerLinear, ReflectionVector, Roughness * SpecularTextureLevels).rgb;

        float2 SpecularBRDF = txIBL_SpecularBRDF_LUT.Sample(samplerClamp, float2(NDotV, Roughness)).rg;
        
        float3 SpecularIBL = (F0 * SpecularBRDF.x + SpecularBRDF.y) * SpecularIrradiance;
        
        AmbientLighting = (DiffuseIBL + SpecularIBL) * SphereAmbient;

    }

    float3 final = DirectionLighting + AmbientLighting + EmissiveLighting; //saturate(directionLighting + BaseColor);
   


    //final = final + (Attenuation * Intensity * (PhongD + PhongS));
    final.rgb = pow(final.rgb, 1 / 2.2);

    return float4(final, 1.0f);


}