#pragma once
#include "StaticMeshResource.h"


class Model
{
protected:
	Math::Matrix m_worldTransform;
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

public:
	virtual void SetSceneResource(shared_ptr<StaticSceneResource> sceneResource)=0;

};

