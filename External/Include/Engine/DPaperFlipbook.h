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
    vector<Ptr<DFlipbook>>  m_vecFlipbook;  // FliBook ������Ʈ�� ������ ��� Flipbook ���
    Ptr<DFlipbook>          m_CurFlipbook;  // ���� ������� Flipbook
    Ptr<DSprite>            m_CurFrmSprite; // ���� ������� Flipbook ���� ���� ������ �ε����� �ش��ϴ� ��������Ʈ
    int                     m_CurFrmIdx;    // ���� ������� Flipbook ���� ���° Sprite �� ��������� �ε��� ���
    float                   m_FPS;          // ���� ������� Flipbook �� �ʴ� ������ ���� ��
    float                   m_AccTime;      // ���� �ð��� üũ
    bool                    m_Repeat;
    bool                    m_Finish;       // Flipbook ����� ���� �����ߴ��� ����

};

#endif // !ENGINE_DPAPER_FLIPBOOK_H