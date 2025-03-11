#ifndef ENGINE_DCOMPUTE_SHADER_H
#define ENGINE_DCOMPUTE_SHADER_H

#include "DShader.h"

class DComputeShader :
    public DShader
{
public:
    DComputeShader(UINT _ThreadPerGroupX, UINT _ThreadPerGroupY, UINT _ThreadPerGroupZ
        , const wstring& _ShaderRelativePath, const string& _FuncName);
    ~DComputeShader();

public:
    int Execute();

protected:
    // Number of threads per group
    const UINT                  m_ThreadPerGroupX;
    const UINT                  m_ThreadPerGroupY;
    const UINT                  m_ThreadPerGroupZ;


    // Group Count
    UINT                        m_GroupX;
    UINT                        m_GroupY;
    UINT                        m_GroupZ;

    // Constant Data pass on usage
    tMtrlConst                  m_Const;

private:
    int CreateComputeShader(const wstring& _RelativePath, const string& _FuncName);

    virtual int Binding() = 0;
    virtual void CalcGroupNum() = 0;
    virtual void Clear() = 0;

private:
    ComPtr<ID3DBlob>            m_CSBlob;
    ComPtr<ID3D11ComputeShader> m_CS;

};

#endif // !ENGINE_DCOMPUTE_SHADER_H