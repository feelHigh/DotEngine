#ifndef ENGINE_DMATERIAL_H
#define ENGINE_DMATERIAL_H

#include "DAsset.h"

#include "Ptr.h"
#include "DTexture.h"
#include "DGraphicShader.h"

class DMaterial :
    public DAsset
{
public:
    CLONE(DMaterial);
    DMaterial(bool _IsEngine = false);
    ~DMaterial();

public:
    void SetShader(Ptr<DGraphicShader> _Shader) { m_Shader = _Shader; }
    Ptr<DGraphicShader> GetShader() { return m_Shader; }

public:
    template<typename T>
    void SetScalarParam(SCALAR_PARAM _Param, const T& _Data);
    void SetTexParam(TEX_PARAM _Param, Ptr<DTexture> _Tex) { m_arrTex[_Param] = _Tex; }

    void* GetScalarParam(SCALAR_PARAM _Param);
    Ptr<DTexture> GetTexParam(TEX_PARAM _Param) { return m_arrTex[(UINT)_Param]; }

public:
    void Binding();

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _RelativePath) override;


private:
    Ptr<DGraphicShader>     m_Shader;

    tMtrlConst              m_Const;
    Ptr<DTexture>           m_arrTex[TEX_PARAM::END];
};

template<typename T>
void DMaterial::SetScalarParam(SCALAR_PARAM _Param, const T& _Data)
{
    if constexpr (std::is_same_v<T, int>) { m_Const.iArr[_Param] = _Data; return; }
    if constexpr (std::is_same_v<T, float>) { m_Const.fArr[_Param - FLOAT_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Vec2>) { m_Const.v2Arr[_Param - VEC2_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Vec4>) { m_Const.v4Arr[_Param - VEC4_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Matrix>) { m_Const.matArr[_Param - MAT_0] = _Data; return; }

    return;
}

#endif // !ENGINE_DMATERIAL_H