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
    vector<Ptr<DFlipbook>>  m_vecFlipbook;  // FliBook 컴포넌트가 보유한 모든 Flipbook 목록
    Ptr<DFlipbook>          m_CurFlipbook;  // 현재 재생중인 Flipbook
    Ptr<DSprite>            m_CurFrmSprite; // 현재 재생중인 Flipbook 에서 현재 프레임 인덱스에 해당하는 스프라이트
    int                     m_CurFrmIdx;    // 현재 재생중인 Flipbook 에서 몇번째 Sprite 가 재생중인지 인덱스 기록
    float                   m_FPS;          // 현재 재생중인 Flipbook 의 초당 프레임 진행 수
    float                   m_AccTime;      // 누적 시간값 체크
    bool                    m_Repeat;
    bool                    m_Finish;       // Flipbook 재생이 끝에 도달했는지 여부

};

#endif // !ENGINE_DPAPER_FLIPBOOK_H