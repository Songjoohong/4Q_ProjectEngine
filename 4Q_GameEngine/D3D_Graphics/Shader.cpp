#include "pch.h"
#include "Shader.h"

bool Shader::SetShader(wstring filename)
{
	//�����ϵ� ���̴� ������ �ִ��� Ȯ�� �ϰ� ������ ���� ��������
	LoadShaderFromFile((filename + L".cso").c_str(), &m_pShader);
	if (m_pShader.Get() == nullptr)
		HR_T(CompileShader(filename));
	return true;
}
