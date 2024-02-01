#pragma once
#include "PixelShader.h"

struct aiMaterial;

class MaterialTexture
{
public:
    MaterialTexture() {}
    ~MaterialTexture(){}

public:
    void Create(const std::wstring& filePath);

public:
    std::wstring m_filePath;
    ComPtr<ID3D11ShaderResourceView> m_pTextureRV = nullptr;
};

class Material
{
public:
    Material(){}
    ~Material(){}

    PixelShader m_pixelShader;
    PixelShader m_shadowPixelShader;

private:
    std::wstring m_fileName;

public:
    shared_ptr<MaterialTexture> m_pDiffuseRV = nullptr;        // �ؽ�ó ���ҽ� ��
    shared_ptr<MaterialTexture> m_pNormalRV = nullptr;         // �ؽ�ó �븻�� ���ҽ� ��                         
    shared_ptr<MaterialTexture> m_pSpecularRV = nullptr;       // �ؽ�ó ����ŧ���� ���ҽ� ��                     
    shared_ptr<MaterialTexture> m_pEmissiveRV = nullptr;       // �ؽ�ó �̹̽ú�� ���ҽ� ��                     
    shared_ptr<MaterialTexture> m_pOpacityRV = nullptr;        // �ؽ�ó ���Ľ�Ƽ�� ���ҽ� ��                     
    shared_ptr<MaterialTexture> m_pMetalicRV = nullptr;        // �ؽ�ó ���Ľ�Ƽ�� ���ҽ� ��                     
    shared_ptr<MaterialTexture> m_pRoughnessRV = nullptr;      // �ؽ�ó ���Ľ�Ƽ�� ���ҽ� ��

public:
    void Create(aiMaterial* material);
    void SetFileName(const std::wstring& fileName);
    
};


