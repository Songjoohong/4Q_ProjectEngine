#pragma once
#include "Shader.h"
#include "D3D_Graphics.h"

class VertexShader :
    public Shader
{
public:
    VertexShader() {}
    VertexShader(std::wstring filename) { SetShader(filename); }
    ~VertexShader() {}

    ComPtr<ID3D11VertexShader> m_pVertexShader;

public:
    virtual bool CompileShader(std::wstring filename);
    virtual bool SetShader(std::wstring filename);
};

