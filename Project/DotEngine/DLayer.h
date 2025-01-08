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
    vector<DGameObject*>    m_Parents; // 해당 레이어 소속 오브젝트중에서 최상위 부모오브젝트만 관리
    vector<DGameObject*>    m_Objects; // 해당 레이어 소속 오브젝트 중에서 부모 자식 상관없는 모든 오브젝트
    const int               m_LayerIdx;

};
