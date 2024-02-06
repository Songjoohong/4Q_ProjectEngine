#pragma once
#include "StaticMeshResource.h"


class Model
{
protected:
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer=nullptr;

public:
	Math::Matrix m_worldTransform = {};
	virtual void SetSceneResource(shared_ptr<StaticSceneResource> sceneResource)=0;

};

