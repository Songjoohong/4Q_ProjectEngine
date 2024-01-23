#include "pch.h"
#include "PixelShader.h"
#include"D3D_Graphics.h"

bool PixelShader::CompileShader(wstring filename)
{
	wstring filePath = ToWString(Renderer::Instance->GetPath()) + filename;
	HR_T(CompileShaderFromFile((filePath + L".hlsl").c_str(), 0, "main", "ps_5_0", m_pShader.GetAddressOf()));
	return true;
}

bool PixelShader::SetShader(wstring filename)
{
	bool isSuccess = false;
	__super::SetShader(filename);
	isSuccess = Renderer::Instance->m_pDevice->CreatePixelShader(m_pShader->GetBufferPointer(), m_pShader->GetBufferSize(), NULL, m_pPixelShader.GetAddressOf());

	return isSuccess;
}
