#pragma once
#include "ImGui\imgui.h"

class DEditor
{
public:
	DEditor();
	virtual ~DEditor();

public:
	virtual void Init() {}
	virtual void Tick();
	virtual void Update() = 0;

public:
	void AddChild(DEditor* _UI);

	DEditor* GetParent() { return m_Parent; }
	const vector<DEditor*>& GetChildren() { return m_vecChildUI; }

	void SetName(const string& _Name);

	const string& GetName() { return m_Name; }

	UINT GetID() { return m_ID; }
	const string& GetFullName() { return m_FullName; }

	bool IsActive() { return m_Active; }

	void UseMenuBar(bool _Use) { m_UseMenuBar = _Use; }
	void SetMove(bool _Move) { m_Move = _Move; }

	void SetChildBorder(bool _Set) { m_ChildBorder = _Set; }
	void SetChildSize(ImVec2 _Size) { m_ChildSize = _Size; }

	void SetActive(bool _Active);

	void SetModal(bool _Modal) { m_Modal = _Modal; }
	bool IsModal() { return m_Modal; }

	void SetFocus();

private:
	virtual void Activate() {}
	virtual void Deactivate() {}

private:
	static UINT			m_GlobalID;

	string				m_Name;
	string				m_FullName;
	const UINT			m_ID;
	bool				m_Active;

	DEditor*			m_Parent;
	vector<DEditor*>	m_vecChildUI;

	ImVec2				m_ChildSize;

	bool				m_Modal;
	bool				m_ChildBorder;
	bool				m_UseMenuBar;
	bool				m_Move;

};

typedef void(DEditor::* DELEGATE_0)(void);
typedef void(DEditor::* DELEGATE_1)(DWORD_PTR);
typedef void(DEditor::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);
typedef void(DEditor::* DELEGATE_3)(DWORD_PTR, DWORD_PTR, DWORD_PTR);