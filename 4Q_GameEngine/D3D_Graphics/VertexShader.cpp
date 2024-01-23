#include "pch.h"
#include"D3D_Graphics.h"
#include "VertexShader.h"
bool VertexShader::CompileShader(wstring filename)
{
	
	wstring filePath = ToWString(Renderer::Instance->GetPath()) + filename + L".hlsl";
	HR_T(CompileShaderFromFile(filePath.c_str(), 0, "main", "vs_5_0", m_pShader.GetAddressOf()));
	
	return true;
}

bool VertexShader::SetShader(wstring filename)
{
	bool isSuccess = false;
	__super::SetShader(filename);
	isSuccess= Renderer::Instance->m_pDevice->CreateVertexShader(m_pShader->GetBufferPointer(), m_pShader->GetBufferSize(), NULL, m_pVertexShader.GetAddressOf());

	return isSuccess;
}
