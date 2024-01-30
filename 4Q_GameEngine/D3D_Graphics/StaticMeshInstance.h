#pragma once
class StaticMeshResource;
class Material;



class StaticMeshInstance
{
public:
	StaticMeshInstance();
	~StaticMeshInstance();

public:
	StaticMeshResource* m_pMeshResource = nullptr;
	Material* m_pMaterial = nullptr;
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh�� ��������� ���� ����� ������

	ComPtr<ID3D11Buffer>m_worldBuffer;
public:
	void Create(StaticMeshResource* pMeshResource, Math::Matrix* pWorld, Material* pMaterial);
	virtual void Render(ID3D11DeviceContext* deviceContext);

	void Initialize();
};