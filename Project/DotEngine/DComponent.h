#ifndef ENGINE_DCOMPONENT_H
#define ENGINE_DCOMPONENT_H

#include "DEntity.h"
#include "DGameObject.h"

#define GET_OTHER_COMPONENT(Type) D##Type* Type() { return m_Owner->Type(); }

class DComponent :
    public DEntity
{
public:
    friend class DGameObject;

public:
    CLONE_DISABLE(DComponent);
    DComponent(COMPONENT_TYPE _Type);
    DComponent(const DComponent& _Origin);
    ~DComponent();

public:
    virtual void Init() {}
    virtual void Begin();
    virtual void Tick();
    virtual void FinalTick() = 0;

public:
    COMPONENT_TYPE GetComponentType() { return m_Type; }
    DGameObject* GetOwner() { return m_Owner; }

public:
    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(PaperFlipbook);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(ParticleSystem);

public:
    virtual void SaveToFile(FILE* _File) = 0;   // Save component information to a file
    virtual void LoadFromFile(FILE* _File) = 0; // Load component information into a file

private:
    void SetOwner(DGameObject* _Object) { m_Owner = _Object; }

private:
    const COMPONENT_TYPE    m_Type;
    DGameObject* m_Owner;

};

#endif // !ENGINE_DCOMPONENT_H