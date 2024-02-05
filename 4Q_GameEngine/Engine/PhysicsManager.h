#pragma once
#include "ECS.h"
#include "ISingleton.h"

#include <map>
#include <vector>

using namespace physx;
using namespace std;

class StaticCollider;
class DynamicCollider;
class PhysicsManager : public ISingleton<PhysicsManager>
{
public:
	PhysicsManager() = default;
	~PhysicsManager() override;

public:
	void Initialize();
	void Update(float deltatime);

	void CreateCollider(BoxCollider* boxcollider, PhysicsType type);
	void DebugSetUp();

	PxPhysics* GetPhysics() { return m_pPhysics; }
	PxScene* GetPxScene() { return m_pPxScene; }
	PxFilterData* GetFilterData(ObjectType type) { return m_pFilterDatas[type]; }
	UserData* GetUserData(ObjectType type) { return m_pUserDatas[type]; }

public:
	PxPvd* m_pPvd = nullptr; // PhysX Visual Debbugger -> �ð�ȭ �Ͽ� ����� �� �� �ִ� ���α׷�
	PxFoundation* m_pFoundation = nullptr;
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;

private:
	PxPhysics* m_pPhysics = nullptr;
	PxScene* m_pPxScene = nullptr;
	ContactModifier* m_pContactModifier;

	// Colliders
	vector<DynamicCollider*> m_pDynamicColliders;
	vector<StaticCollider*> m_pStaticColliders;
	unordered_map<ObjectType,PxFilterData*> m_pFilterDatas;
	unordered_map< ObjectType,UserData*> m_pUserDatas;
};


/*
*  ���� : ContactModifyCallback -> �浹�� �ڵ�ȣ��
*/

struct UserData
{
	ObjectType m_Type;
};

class ContactModifier
	:public PxContactModifyCallback
{
public:
	ContactModifier() = default;
	~ContactModifier() = default;

public:
	virtual void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count)
	{
		for (physx::PxU32 i = 0; i < count; ++i)
		{
			const physx::PxContactModifyPair& modifyPair = pairs[i];

			// ���� : �浹�� �� ��ü�� UserData ���� ��������
			UserData* userData0 = reinterpret_cast<UserData*>(modifyPair.actor[0]->userData);
			UserData* userData1 = reinterpret_cast<UserData*>(modifyPair.actor[1]->userData);
			ObjectType type = userData0->m_Type;

			if (userData0 && userData1)
			{
				// ���� : �浹�� ������ ����� �ۼ��ϱ�

				if (userData0 && userData1 &&
					((userData0->m_Type == ObjectType::PLAYER && userData1->m_Type == ObjectType::SLOPE) ||
						(userData0->m_Type == ObjectType::SLOPE && userData1->m_Type == ObjectType::PLAYER)))
				{
					physx::PxRigidDynamic* playerActor = (PxRigidDynamic*)(modifyPair.actor[0]);
					if (playerActor)
					{
						// ���⼭ playerActor�� ���ϴ� ���ӵ��� �����ϴ� ���� �۾��� ������ ����

					}
				}
				else
				{

				}
			}
		}
	}

};
