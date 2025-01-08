#pragma once
#include <Engine\DScript.h>

class PlayerScript :
    public DScript
{
public:
    CLONE(PlayerScript);
    PlayerScript();
    ~PlayerScript();

public:
    virtual void Begin() override;
    virtual void Tick() override;

public:
    virtual void BeginOverlap(DCollider2D* _OwnCollider, DGameObject* _OtherObject, DCollider2D* _OtherCollider) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    float           m_Speed;
    Ptr<DTexture>   m_Texture;
    Ptr<DPrefab>    m_MissilePref;

};
