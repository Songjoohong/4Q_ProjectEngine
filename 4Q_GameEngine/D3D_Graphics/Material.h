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
    shared_ptr<MaterialTexture> m_pDiffuseRV = nullptr;        // ÅØ½ºÃ³ ¸®¼Ò½º ºä
    shared_ptr<MaterialTexture> m_pNormalRV = nullptr;         // ÅØ½ºÃ³ ³ë¸»¸Ê ¸®¼Ò½º ºä                         
    shared_ptr<MaterialTexture> m_pSpecularRV = nullptr;       // ÅØ½ºÃ³ ½ºÆåÅ§·¯¸Ê ¸®¼Ò½º ºä                     
    shared_ptr<MaterialTexture> m_pEmissiveRV = nullptr;       // ÅØ½ºÃ³ ÀÌ¹Ì½Ãºê¸Ê ¸®¼Ò½º ºä                     
    shared_ptr<MaterialTexture> m_pOpacityRV = nullptr;        // ÅØ½ºÃ³ ¿ÀÆÄ½ÃÆ¼¸Ê ¸®¼Ò½º ºä                     
    shared_ptr<MaterialTexture> m_pMetalicRV = nullptr;        // ÅØ½ºÃ³ ¿ÀÆÄ½ÃÆ¼¸Ê ¸®¼Ò½º ºä                     
    shared_ptr<MaterialTexture> m_pRoughnessRV = nullptr;      // ÅØ½ºÃ³ ¿ÀÆÄ½ÃÆ¼¸Ê ¸®¼Ò½º ºä

public:
    void Create(aiMaterial* material);
    void SetFileName(const std::wstring& fileName);
    
};


