#pragma once
#include "DEditor.h"

class Outliner :
    public DEditor
{
public:
    Outliner();
    ~Outliner();

public:
    virtual void Update() override;

private:
    void RenewLevel();
    void GameObjectClicked(DWORD_PTR _Param);
    void AddGameObject(class TreeNode* pNode, class DGameObject* _Object);

    void GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2);
    void DroppedFromOuter(DWORD_PTR _OuterData, DWORD_PTR _DropNode);

private:
    class TreeUI* m_Tree;

};
