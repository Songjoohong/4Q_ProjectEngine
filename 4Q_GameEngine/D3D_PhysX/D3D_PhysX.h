#pragma once
#include <vector>

using namespace physx;
using namespace std;

/*
	//����//
	Todo : ����� ���� �ص־��ϴ� �͵�
	1. PX_RELEASE ���ֱ�.
	2. ���� ������Ʈ �� �ٲ𶧸��� ���� / �߰� ��� �ϱ�.
*/

class Model;
class StaticRigidInstance;
class DynamicRigidInstance;
class Physics
{
public:
	~Physics()=default;

public:
	void Initialize();
	void Update();

	Physics* GetInstance();
	void StepPhysics();

private:
	vector<StaticRigidInstance*> m_StaticRigidInstances;
	vector<DynamicRigidInstance*> m_DynamicRigidInstances;

private:
	//PxPvd* m_pPvd = nullptr; // PhysX Visual Debbugger -> �ð�ȭ �Ͽ� ����� �� �� �ִ� ���α׷�?
	PxFoundation*			m_pFoundation = nullptr;
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;

	PxPhysics*			    m_pPhysics = nullptr;
	PxScene*				m_pScene = nullptr;
	PxMaterial*				m_pMaterial = nullptr;

	static Physics*			m_pInstance;

	Physics() = default;
};