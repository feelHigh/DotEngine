#ifndef ENGINE_DFLIPBOOK_H
#define ENGINE_DFLIPBOOK_H

#include "DAsset.h"
#include "DSprite.h"

class DFlipbook :
    public DAsset
{
public:
    DFlipbook();
    ~DFlipbook();

public:
    void FinalTick();

public:
    void AddSprite(Ptr<DSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    Ptr<DSprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }
    int GetMaxFrameCount() { return (int)m_vecSprite.size(); }

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    vector<Ptr<DSprite>>    m_vecSprite;

};

#endif // !ENGINE_DFLIPBOOK_H