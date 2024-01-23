#include "pch.h"
#include "Shader.h"

bool Shader::SetShader(wstring filename)
{
	//컴파일된 셰이더 파일이 있는지 확인 하고 없으면 새로 컴파일함
	LoadShaderFromFile((filename + L".cso").c_str(), &m_pShader);
	if (m_pShader.Get() == nullptr)
		HR_T(CompileShader(filename));
	return true;
}
