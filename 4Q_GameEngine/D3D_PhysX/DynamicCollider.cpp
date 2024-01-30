#include "pch.h"
#include "DynamicCollider.h"

void DynamicCollider::Update()
{
	PxTransform trans = m_Rigid->getGlobalPose();
	//어떻게 이 정보를 넘겨주지..
}
