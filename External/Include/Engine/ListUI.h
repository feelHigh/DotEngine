#pragma once
#include "DEditor.h"

class ListUI :
    public DEditor
{
public:
    ListUI();
    ~ListUI();

public:
    virtual void Update() override;

public:
    void AddItem(const string& _item);
    void AddList(const vector<string>& _list);
    void AddDelegate(DEditor* _UI, DELEGATE_1 _Func)
    {
        m_UI = _UI;
        m_MemFunc = _Func;
    }

    string GetSelectName() { return m_SelectedName; }

    virtual void Activate() override;
    virtual void Deactivate() override;

private:
    vector<string>  m_vecList;
    int             m_SelectedIdx;
    string          m_SelectedName;


    DEditor*        m_UI;
    DELEGATE_1      m_MemFunc;

};
