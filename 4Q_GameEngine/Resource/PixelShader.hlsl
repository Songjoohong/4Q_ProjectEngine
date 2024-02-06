#include "Header.hlsli"

static const float Epsilon = 0.00001;
static const float PI= 3.141592;

//비금속 프레넥 계수
static const float3 Fdielectric = 0.04;

//법선 분포 함수 : 노말 벡터와 하프벡터 사이각이 작을 수록 반사율이 커지고 러프니스가 커질 수록 반사율이 작아진다.
float NDF(float NdotH,float roughness)  //노말벡터와 하프벡터를 내적 연산한 값과 러프니스를 매개변수로 받아옴
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float denominator = (NdotH * NdotH) * (alphaSq - 1.0f) + 1.0f;
    return alphaSq / (PI * denominator * denominator);
}
//프레넬 반사 함수 
float3 FresnelSchlick(float3 F0, float HdotV)    //F0 : 최저 반사율 lerp(비금속 프레넬 계수,BaseColor,metalness) HdotV : 하프벡터와 뷰벡터를 내적 연산한 값
{
    return F0 + (1.0f - F0) * pow(1.0f - HdotV, 5.0f);
}

//GGX연산 할 G함수
float GSub(float cosTheta,float k)
{
    return cosTheta / (cosTheta * (1.0f - k) + k);
}

//기하학적 감쇠 함수
float GGX(float NdoL,float NdotV,float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    return GSub(NdoL, k) * GSub(NdotV, k);
}



float4 main(PS_INPUT input) : SV_TARGET
{
    //기본적인 변수 설정
    float4 BaseColor = txDiffuse.Sample(samplerLinear, input.Texcoord);
    float3 Normal = normalize(input.NorWorld);
    float3 Tangent = normalize(input.TanWorld);
    float3 BiTangent = cross(input.TanWorld, input.NorWorld);
    float3 LightDirection = -normalize(Direction.xyz);
    float3 ViewVector = normalize(CameraPos - input.PosWorld);
    float3 HalfVector = normalize(LightDirection + ViewVector);
    
    float3 EmissiveLighting = txEmissive.Sample(samplerLinear, input.Texcoord);
    
    float AmbientOcclusion = txAmbient.Sample(samplerLinear, input.Texcoord);
    
    //Normal Tangent space (노말맵이 있을경우)
    float3 NormalTangent = txNormal.Sample(samplerLinear, input.Texcoord);
    float3x3 WorldTransform = float3x3(Tangent, BiTangent, Normal);
    Normal = mul(NormalTangent, WorldTransform);
    Normal = normalize(Normal);
    
    //dot production
    //노말과 라이트디렉션의 각
    float NDotL = max(0, dot(Normal, LightDirection));
    //노말과 하프벡터의 각
    float NDotH = max(0, dot(Normal, HalfVector));
    //노말과 뷰벡터의 각
    float NDotV = max(0, dot(Normal, ViewVector));
    
    //Gamma correction
    BaseColor.rgb = pow(BaseColor.rgb, 2.2);
    
    
    float Metalness = 1.0f;
    Metalness = txMetalic.Sample(samplerLinear, input.Texcoord).r;
    
    float Roughness = 0.0f;
    Roughness = txRoughness.Sample(samplerLinear, input.Texcoord).r;
    
    float3 Emissive = 0.0f;
    Emissive = txEmissive.Sample(samplerLinear, input.Texcoord).rgb;
    
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
    
    if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0)
    {
        float sampleShadowDepth = txShadow.Sample(samplerLinear, uv).r;
		// currentShadowDepth�� ũ�� �� ���ʿ� �����Ƿ� �������� ���ܵȴ�.
        if (currentShadowDepth > sampleShadowDepth + 0.001)
        {
            //DirectionLighting = 0.0f;
        }
    }
    
    
    //DirectionLighting += (DiffuseBRDF + SpecularBRDF) * LightColor * NDotL;
    float3 PointLighting = 0.0;
    
    float3 LightVector = LightPos - input.PosWorld;
    float4 light=1.f;
    float len = length(LightVector);
    float att = 1;
    if (len < Radius)
    {
        float3 NL = LightVector / len;
        F = FresnelSchlick(F0, max(0, dot(NL + ViewVector, ViewVector)));
        D = NDF(max(0, dot(Normal, NL + ViewVector)), Roughness);
        G = GGX(max(0, dot(Normal, NL)), NDotV, Roughness);
        
        Kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), Metalness);
        DiffuseBRDF = Kd * BaseColor.rgb;
        
        SpecularBRDF = (F * D * G) / max(Epsilon, 4.0 * dot(Normal, NL) * NDotV);
        float pow = 2000 / (len * len);
        PointLighting += (DiffuseBRDF + SpecularBRDF) * pow;
    }
    
    //IBL
    float3 AmbientLighting = 0.0f;
    
    {
        float3 ReflectionVector = 2.0 * NDotV * Normal - ViewVector;
        float3 Irradiance = txIBL_Diffuse.Sample(samplerLinear, Normal).rgb;
        
        //표면에서 받는 빛 방향을 특정 할 수 없어서 하프벡터와 내적한 값을 사용하지 않고 노말벡터와 시야벡터를 내적해서 사용한다.
        float3 F = FresnelSchlick(F0, NDotV);
        
        float3 Kd = lerp(1.9 - F, 0.0, Metalness);
        
        float3 DiffuseIBL = Kd * BaseColor.rgb * Irradiance;
        
        uint Width,Height,SpecularTextureLevels;
        
        txIBL_Specular.GetDimensions(0, Width, Height, SpecularTextureLevels);
        
        float3 SpecularIrradiance = txIBL_Specular.SampleLevel(samplerLinear, ReflectionVector, Roughness*SpecularTextureLevels).rgb;

        float2 SpecularBRDF = txIBL_SpecularBRDF_LUT.Sample(samplerClamp, float2(NDotV, Roughness)).rg;
        
        float3 SpecularIBL = (F0 * SpecularBRDF.x + SpecularBRDF.y) * SpecularIrradiance;
        
        AmbientLighting = (DiffuseIBL + SpecularIBL) * AmbientOcclusion;

    }

    float3 final = DirectionLighting + PointLighting + AmbientLighting + EmissiveLighting; //saturate(directionLighting + BaseColor);
    final.rgb = pow(final.rgb, 1 / 2.2);
    return float4(final.rgb, 1.0f);

}