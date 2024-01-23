#pragma once
#include "Shader.h"
#include "D3D_Graphics.h"

class VertexShader :
    public Shader
{
public:
    VertexShader() {}
    VertexShader(wstring filename) { SetShader(filename); }
    ~VertexShader() {}

    ComPtr<ID3D11VertexShader> m_pVertexShader;

public:
    virtual bool CompileShader(wstring filename);
    virtual bool SetShader(wstring filename);
};

