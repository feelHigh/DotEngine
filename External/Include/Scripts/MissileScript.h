#pragma once
#include <Engine\DScript.h>

class MissileScript :
    public DScript
{
public:
    CLONE(MissileScript);
    MissileScript();
    ~MissileScript();

public:
    virtual void Begin() override;
    virtual void Tick() override;

public:
    virtual void BeginOverlap(DCollider2D* _OwnCollider, DGameObject* _OtherObject, DCollider2D* _OtherCollider) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    float   m_Speed;

};
