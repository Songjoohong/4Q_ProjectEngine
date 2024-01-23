#include "pch.h"
#include "InputLayout.h"

void InputLayout::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* desc, ComPtr<ID3DBlob> vs, UINT size)
{
	HR_T(Renderer::Instance->m_pDevice->CreateInputLayout(desc, size, vs->GetBufferPointer(), vs->GetBufferSize(), m_pInputLayout.GetAddressOf()));
}
