#ifndef ENGINE_DPREFAB_H
#define ENGINE_DPREFAB_H

#include "DAsset.h"
#include "DEngine.h"

class DPrefab :
    public DAsset
{
public:
    friend class DEngine;

public:
    DPrefab();
    ~DPrefab();

public:
    void SetProtoObject(DGameObject* _Object) { m_ProtoObject = _Object; }
    DGameObject* Instantiate();

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    static OBJECT_SAVE  g_ObjectSaveFunc;
    static OBJECT_LOAD  g_ObjectLoadFunc;

private:
    class DGameObject* m_ProtoObject;

};

#endif // !ENGINE_DPREFAB_H