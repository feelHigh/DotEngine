#ifndef ENGINE_GAMEOBJECT_H
#define ENGINE_GAMEOBJECT_H

#include "DEntity.h"

class DComponent;
class DRenderComponent;
class DScript;

#define GET_COMPONENT(Type, TYPE) class D##Type* Type() { return (D##Type*)GetComponent(COMPONENT_TYPE::TYPE); }

class DGameObject :
    public DEntity
{
public:
    friend class DLevel;
    friend class DLayer;
    friend class DTaskMgr;

public:
    CLONE(DGameObject);
    DGameObject();
    DGameObject(const DGameObject& _Origin);
    ~DGameObject();

public:
    void Begin();
    void Tick();
    virtual void FinalTick();
    void Render();

public:
    void AddComponent(DComponent* _Comopnent);
    DComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    DRenderComponent* GetRenderComponent() { return m_RenderCom; }

    DGameObject* GetParent() { return m_Parent; }
    int GetLayerIdx() { return m_LayerIdx; }
    void AddChild(DGameObject* _ChildObject);
    const vector<DGameObject*>& GetChildren() { return m_vecChildren; }
    const vector<DScript*> GetScripts() { return m_vecScript; }


    bool IsAncestor(DGameObject* _Object);
    bool IsDead() { return m_Dead; }

    void DisconnectWithLayer();

    // Unregister : 등록을 취소하다, 등록하는 행위를 취소하다
    // Deregister : 등록된것을 취소하다
    void DeregisterChild();

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(PaperFlipbook, PAPERFLIPBOOK);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);

private:
    DComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];
    DRenderComponent* m_RenderCom;
    vector<DScript*>        m_vecScript;

    DGameObject* m_Parent;
    vector<DGameObject*>    m_vecChildren;

    int                     m_LayerIdx; // 소속 레이어의 인덱스 번호
    bool                    m_Dead;

};

#endif // !ENGINE_GAMEOBJECT_H