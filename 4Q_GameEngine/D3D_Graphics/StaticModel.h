#pragma once
#include "Model.h"
#include "Material.h"

#include "StaticMeshInstance.h"

class StaticModel :
    public Model
{
private:
    shared_ptr<StaticSceneResource> m_pStaticSceneResource;
    std::vector<StaticMeshInstance> m_meshInstance;

public:
    StaticModel() {}
    virtual ~StaticModel();
    virtual void SetSceneResource(shared_ptr<StaticSceneResource> sceneResource);
    bool Load(string filename);
    void RenderInit();
    void Render();
};

