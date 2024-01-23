#include "pch.h"
#include "StaticMeshInstance.h"
#include "StaticMeshResource.h"
#include "D3D_Graphics.h"


StaticMeshInstance::StaticMeshInstance()
{
}

StaticMeshInstance::~StaticMeshInstance()
{

}


void StaticMeshInstance::Create(StaticMeshResource* pMeshResource, Math::Matrix* pWorld, Material* pMaterial)
{
	m_pMeshResource = pMeshResource;
	m_pMaterial = pMaterial;
	m_pNodeWorldTransform = pWorld;
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = static_cast<UINT>(sizeof(Math::Matrix));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	HR_T(Renderer::Instance->m_pDevice->CreateBuffer(&bd, nullptr, &m_worldBuffer));
}

void StaticMeshInstance::Render(ID3D11DeviceContext* deviceContext)
{
	Renderer::Instance->m_pDeviceContext->IASetVertexBuffers(0, 1, m_pMeshResource->m_pVertexBuffer.GetAddressOf(), &m_pMeshResource->m_vertexBufferStride, &m_pMeshResource->m_vertexBufferOffset);
	Renderer::Instance->m_pDeviceContext->IASetIndexBuffer(m_pMeshResource->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->UpdateSubresource(m_worldBuffer.Get(), 0, nullptr, &m_worldTransfromCB, 0, 0);
	deviceContext->VSSetConstantBuffers(2, 1, m_worldBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(2, 1, m_worldBuffer.GetAddressOf());
	deviceContext->DrawIndexed(m_pMeshResource->m_indexCount, 0, 0);
}

void StaticMeshInstance::Initialize()
{
	Renderer::Instance->m_pDeviceContext->IASetInputLayout(m_pMeshResource->m_inputLayout.m_pInputLayout.Get());
	Renderer::Instance->m_pDeviceContext->IASetVertexBuffers(0, 1, m_pMeshResource->m_pVertexBuffer.GetAddressOf(), &m_pMeshResource->m_vertexBufferStride, &m_pMeshResource->m_vertexBufferOffset);
	Renderer::Instance->m_pDeviceContext->IASetIndexBuffer(m_pMeshResource->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Renderer::Instance->m_pDeviceContext->VSSetShader(m_pMeshResource->m_vertexShader.m_pVertexShader.Get(), nullptr, 0);
}
