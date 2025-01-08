#ifndef ENGINE_DSCRIPT_H
#define ENGINE_DSCRIPT_H

#include "DComponent.h"

#include "ComponentList.h"
#include "DTimeMgr.h"
#include "DKeyMgr.h"
#include "DAssetMgr.h"

enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    TEXTURE,
    PREFAB,
};

struct tScriptParam
{
    SCRIPT_PARAM    Type;
    void* pData;
    string          Desc;
    DWORD_PTR       Param_0;
    DWORD_PTR       Param_1;
};


class DScript :
    public DComponent
{
public:
    DScript(UINT _Type);
    ~DScript();

public:
    virtual void Begin() override {}
    virtual void Tick() = 0;
    virtual void FinalTick() final override {}

public:
    virtual void BeginOverlap(DCollider2D* _OwnCollider, DGameObject* _OtherObject, DCollider2D* _OtherCollider) {}
    virtual void Overlap(DCollider2D* _OwnCollider, DGameObject* _OtherObject, DCollider2D* _OtherCollider) {}
    virtual void EndOverlap(DCollider2D* _OwnCollider, DGameObject* _OtherObject, DCollider2D* _OtherCollider) {}

public:
    DRenderComponent* GetRenderComponent() { return GetOwner()->GetRenderComponent(); }
    UINT GetScriptType() { return m_ScriptType; }
    const vector<tScriptParam>& GetScriptParam() { return   m_ScriptParam; }

public:
    virtual void SaveToFile(FILE* _File) = 0;
    virtual void LoadFromFile(FILE* _File) = 0;

protected:
    void AddScriptParam(SCRIPT_PARAM _Type, const string& _Desc, void* _pData, DWORD_PTR _Param0 = 0, DWORD_PTR _Param1 = 0)
    {
        m_ScriptParam.push_back(tScriptParam{ _Type,_pData, _Desc, _Param0, _Param1 });
    }

    void Instantiate(Ptr<DPrefab> _Pref, int _LayerIdx, Vec3 _WorldPos, const wstring& _Name = L"");

private:
    UINT                    m_ScriptType;
    vector<tScriptParam>    m_ScriptParam;

};

#endif // !ENGINE_DSCRIPT_H