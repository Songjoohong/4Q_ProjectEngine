#pragma once
#include "Model.h"
#include "Material.h"

#include "StaticMeshInstance.h"

class StaticModel :
    public Model
{
private:
    shared_ptr<StaticSceneResource> m_pStaticSceneResource=nullptr;

public:
    DirectX::BoundingBox m_boundingBox;
    std::vector<StaticMeshInstance> m_meshInstance = {};
    bool m_bIsCulled=false;

    StaticModel() {}
    StaticModel(const StaticModel& obj) {}
    StaticModel(const StaticModel&& obj) noexcept {}
    virtual ~StaticModel();

    virtual void SetSceneResource(shared_ptr<StaticSceneResource> sceneResource);
    shared_ptr<StaticSceneResource> GetSceneResource() { return m_pStaticSceneResource; }
    bool Load(string filename);

    void MeshInstancePushBack();
    void RenderInit();
};

