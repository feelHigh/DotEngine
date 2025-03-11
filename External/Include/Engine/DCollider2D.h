#ifndef ENGINE_DCOLLIDER2D_H
#define ENGINE_DCOLLIDER2D_H

#include "DComponent.h"

class DCollider2D :
    public DComponent
{
public:
    CLONE(DCollider2D);
    DCollider2D();
    DCollider2D(const DCollider2D& _Origin);
    ~DCollider2D();

public:
    virtual void FinalTick() override;

public:
    void SetOffset(Vec3 _Offset) { m_Offset = _Offset; }
    void SetScale(Vec3 _Scale) { m_Scale = _Scale; }
    void SetIndependentScale(bool _Set) { m_IndependentScale = _Set; }

    Vec3 GetOffset() { return m_Offset; }
    Vec3 GetScale() { return m_Scale; }
    Vec3 GetWorldPos() { return m_matColWorld.Translation(); }
    const Matrix& GetWorldMat() { return m_matColWorld; }

    int GetOverlapCount() { return m_OverlapCount; }

public:
    virtual void BeginOverlap(DCollider2D* _Other);
    virtual void Overlap(DCollider2D* _Other);
    virtual void EndOverlap(DCollider2D* _Other);

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Vec3    m_Offset;
    Vec3    m_Scale;
    Matrix  m_matColWorld;      // Final world state of the collider
    int     m_OverlapCount;

    bool    m_IndependentScale;

};

#endif // !ENGINE_DCOLLIDER2D_H