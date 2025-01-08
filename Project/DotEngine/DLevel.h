#pragma once
#include "DEntity.h"
class DLayer;
class DGameObject;

class DLevel :
    public DEntity
{
public:
    friend class DTaskMgr;

public:
    CLONE(DLevel);
    DLevel();
    DLevel(const DLevel& _Origin);
    ~DLevel();

public:
    void Begin();   // 레벨이 시작되는 순간 or 레벨에 추가(합류) 하는 순간
    void Tick();
    void FinalTick();

public:
    void ClearObject();

public:
    void AddObject(int LayerIdx, DGameObject* _Object, bool _bMoveChild = false);
    void RegisterAsParent(int LayerIdx, DGameObject* _Object);

    DLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    LEVEL_STATE GetState() { return m_State; }
    DGameObject* FindObjectByName(const wstring& _Name);

private:
    void ChangeState(LEVEL_STATE _NextState);

private:
    DLayer* m_Layer[MAX_LAYER];
    LEVEL_STATE     m_State;

};
