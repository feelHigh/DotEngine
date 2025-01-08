#pragma once
#include "DEditor.h"

class TreeNode
{
public:
    friend class TreeUI;

public:
    TreeNode(UINT _ID);
    ~TreeNode();

public:
    void Update();

public:
    void SetName(const string& _Name) { m_Name = _Name; }
    const string& GetName() { return m_Name; }

    void SetFrame(bool _Frame) { m_Frame = _Frame; }
    bool IsFrame() { return m_Frame; }

    DWORD_PTR GetData() { return m_Data; }

    void AddChildNode(TreeNode* _Node) { m_vecChildNode.push_back(_Node); }


    void DragCheck();
    void DropCheck();

private:
    class TreeUI*       m_Owner;

    string              m_Name;
    TreeNode*           m_ParentNode;
    vector<TreeNode*>   m_vecChildNode;
    const UINT          m_ID;
    DWORD_PTR           m_Data;

    bool                m_Frame;
    bool                m_Selected;
};

class TreeUI :
    public DEditor
{
public:
    TreeUI();
    ~TreeUI();

public:
    virtual void Update() override;

public:
    TreeNode* AddNode(TreeNode* _Parent, const string& _Name, DWORD_PTR _Data = 0);
    void ShowRoot(bool _Show) { m_ShowRoot = _Show; }
    bool IsShowRoot() { return m_ShowRoot; }

    void ShowNameOnly(bool _NameOnly) { m_ShowNameOnly = _NameOnly; }
    bool IsShowNameOnly() { return m_ShowNameOnly; }

    void SetSelectedNode(TreeNode* _Node);
    void SetDragedNode(TreeNode* _Node);
    void SetDroppedNode(TreeNode* _Node);
    void PopupMenu(TreeNode* _Node);

    void UseDrag(bool _Drag) { m_UseDrag = _Drag; }
    void UseDrop(bool _Drop) { m_UseDrop = _Drop; }

    bool IsDrag() { return m_UseDrag; }
    bool IsDrop() { return m_UseDrop; }

    void SetDropPayLoadName(const string& _Name) { m_DropPayLoadName = _Name; }
    const string GetDropPayLoadName() { return m_DropPayLoadName; }

    void AddClickedDelegate(DEditor* _Inst, DELEGATE_1 _Func) { m_ClickedInst = _Inst; m_ClickedFunc = _Func; }
    void AddDragDropDelegate(DEditor* _Inst, DELEGATE_2 _Func) { m_SelfDragDropInst = _Inst; m_SelfDragDropFunc = _Func; }
    void AddDropDelegate(DEditor* _Inst, DELEGATE_2 _Func) { m_DropInst = _Inst; m_DropFunc = _Func; }
    void AddPopupDelegate(DEditor* _Inst, DELEGATE_1 _Func) { m_PopupInst = _Inst; m_PopupFunc = _Func; }

public:
    void Clear();

private:
    TreeNode* m_Root;
    TreeNode* m_SelectedNode;
    TreeNode* m_DragedNode;
    TreeNode* m_DroppedNode;



    UINT        m_NodeID;
    bool        m_ShowRoot;
    bool        m_ShowNameOnly;
    bool        m_UseDrag;
    bool        m_UseDrop;

    DEditor*    m_ClickedInst;
    DELEGATE_1  m_ClickedFunc;

    DEditor*    m_SelfDragDropInst;
    DELEGATE_2  m_SelfDragDropFunc;

    DEditor*    m_DropInst;
    DELEGATE_2  m_DropFunc;
    string      m_DropPayLoadName;

    DEditor*    m_PopupInst;
    DELEGATE_1  m_PopupFunc;

};
