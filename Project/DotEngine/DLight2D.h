#ifndef ENGINE_DLIGHT2D_H
#define ENGINE_DLIGHT2D_H

#include "DComponent.h"

class DLight2D :
    public DComponent
{
public:
    CLONE(DLight2D);
    DLight2D();
    ~DLight2D();

public:
    virtual void FinalTick() override;

public:
    void SetLightType(LIGHT_TYPE _Type);
    void SetRadius(float _Radius) { m_Info.Radius = _Radius; }
    void SetAngle(float _Angle) { m_Info.Angle = _Angle; }
    void SetLightColor(Vec3 _Color) { m_Info.LightEssence.Color = _Color; }
    void SetLightAmbient(Vec3 _Amb) { m_Info.LightEssence.Ambient = _Amb; }

    LIGHT_TYPE GetLightType() { return m_Info.Type; }
    float GetRadius() { return m_Info.Radius; }
    float GetAngle() { return m_Info.Angle; }
    Vec3 GetLightColor() { return m_Info.LightEssence.Color; }
    Vec3 GetLightAmbient() { return m_Info.LightEssence.Ambient; }

    const tLightInfo& GetLightInfo() { return m_Info; }

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    tLightInfo      m_Info;

};

#endif // !ENGINE_DLIGHT2D_H