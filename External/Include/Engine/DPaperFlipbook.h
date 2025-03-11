#ifndef ENGINE_DPAPER_FLIPBOOK_H
#define ENGINE_DPAPER_FLIPBOOK_H

#include "DComponent.h"
#include "DFlipbook.h"

class DPaperFlipbook :
    public DComponent
{
public:
    CLONE(DPaperFlipbook);
    DPaperFlipbook();
    DPaperFlipbook(DPaperFlipbook& _Origin);
    ~DPaperFlipbook();

public:
    virtual void FinalTick() override;

public:
    void AddFlipbook(int _Idx, Ptr<DFlipbook> _Flipbook);
    Ptr<DFlipbook> FindFlipbook(const wstring& _Key);
    void Play(int _FliBookIdx, float _FPS, bool _Repeat);
    Ptr<DSprite> GetCurSprite() { return m_CurFrmSprite; }

    void Reset();
    void Binding();
    static void Clear();

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    vector<Ptr<DFlipbook>>  m_vecFlipbook;  // List of all Flipbooks held by Flipbook components
    Ptr<DFlipbook>          m_CurFlipbook;  // Flipbook currently playing
    Ptr<DSprite>            m_CurFrmSprite; // Sprite corresponding to the current frame index in the Flipbook that is currently playing
    int                     m_CurFrmIdx;    // Indexing how many Sprites are playing in Flipbook that is currently playing
    float                   m_FPS;          // Frame progress per second for Flipbook currently playing
    float                   m_AccTime;      // Check cumulative time values
    bool                    m_Repeat;
    bool                    m_Finish;       // Whether Flipbook playback has reached its end

};

#endif // !ENGINE_DPAPER_FLIPBOOK_H