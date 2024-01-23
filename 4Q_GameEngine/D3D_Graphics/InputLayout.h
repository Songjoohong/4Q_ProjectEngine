#pragma once
#include"D3D_Graphics.h"

class InputLayout
{
public:
	ComPtr<ID3D11InputLayout> m_pInputLayout;

public:
	void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* desc, ComPtr<ID3DBlob> vs, UINT size);
};

