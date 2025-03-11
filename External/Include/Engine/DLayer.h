#pragma once
#include "DEntity.h"

class DGameObject;

class DLayer :
    public DEntity
{
public:
    CLONE(DLayer);
    DLayer(int _LayerIdx);
    DLayer(const DLayer& _Origin);
    ~DLayer();

public:
    void Begin();
    void Tick();
    void FinalTick();

public:
    int GetLayerIdx() { return m_LayerIdx; }
    void AddObject(DGameObject* _Object, bool _bMoveChild);
    const vector<DGameObject*>& GetParentObjects() { return m_Parents; }
    const vector<DGameObject*>& GetObjects() { return m_Objects; }
    void ClearObject() { m_Objects.clear(); }
    void RegisterGameObject(DGameObject* _Object) { m_Objects.push_back(_Object); }
    void DisconnectWithObject(DGameObject* _Object);

    void RegisterAsParent(DGameObject* _Object);
    void DeregisterObjectAsParent(DGameObject* _Object);
    void DeregisterObject(DGameObject* _Object);

private:
    vector<DGameObject*>    m_Parents; // Manage only top parent objects among objects belonging to that layer
    vector<DGameObject*>    m_Objects; // All objects belonging to the corresponding layer that are irrelevant to the parent or child
    const int               m_LayerIdx;

};
