#pragma once
#include "D3D_Graphics.h"

class Shader
{
public:
	Shader() {};
	~Shader() {};

	//���� ���� �Լ��� ���� Shader�� ����� vertexShader�� pixelShader�� �������̵��� �Լ� ���
	virtual bool CompileShader(std::wstring filename) = 0;
	virtual bool SetShader(std::wstring filename);

	ComPtr<ID3DBlob> GetShader() { return m_pShader; }
public:
	ComPtr<ID3DBlob> m_pShader;

};

