#pragma once
#include "D3D_Graphics.h"

class Shader
{
public:
	Shader() {};
	~Shader() {};

	//순수 가상 함수로 만들어서 Shader를 상속한 vertexShader나 pixelShader의 오버라이딩된 함수 사용
	virtual bool CompileShader(std::wstring filename) = 0;
	virtual bool SetShader(std::wstring filename);

	ComPtr<ID3DBlob> GetShader() { return m_pShader; }
public:
	ComPtr<ID3DBlob> m_pShader;

};

