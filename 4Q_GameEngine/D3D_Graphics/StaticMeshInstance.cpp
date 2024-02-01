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
	
}

void StaticMeshInstance::Render(ID3D11DeviceContext* deviceContext)
{
	Renderer::Instance->m_pDeviceContext->IASetVertexBuffers(0, 1, m_pMeshResource->m_pVertexBuffer.GetAddressOf(), &m_pMeshResource->m_vertexBufferStride, &m_pMeshResource->m_vertexBufferOffset);
	Renderer::Instance->m_pDeviceContext->IASetIndexBuffer(m_pMeshResource->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	
	deviceContext->DrawIndexed(m_pMeshResource->m_indexCount, 0, 0);
}

void StaticMeshInstance::Initialize()
{
	Renderer::Instance->m_pDeviceContext->IASetInputLayout(m_pMeshResource->m_inputLayout.m_pInputLayout.Get());
	Renderer::Instance->m_pDeviceContext->IASetVertexBuffers(0, 1, m_pMeshResource->m_pVertexBuffer.GetAddressOf(), &m_pMeshResource->m_vertexBufferStride, &m_pMeshResource->m_vertexBufferOffset);
	Renderer::Instance->m_pDeviceContext->IASetIndexBuffer(m_pMeshResource->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Renderer::Instance->m_pDeviceContext->VSSetShader(m_pMeshResource->m_vertexShader.m_pVertexShader.Get(), nullptr, 0);
}
