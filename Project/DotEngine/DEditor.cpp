#include "pch.h"
#include "DEditor.h"

UINT DEditor::m_GlobalID = 0;

DEditor::DEditor()
	: m_Active(true)
	, m_Parent(nullptr)
	, m_ID(m_GlobalID++)
	, m_Modal(false)
	, m_ChildBorder(false)
	, m_UseMenuBar(false)
	, m_Move(true)
{
}

DEditor::~DEditor()
{
	Delete_Vec(m_vecChildUI);
}

void DEditor::Tick()
{
	if (!m_Active)
		return;

	bool bActive = m_Active;

	UINT flag = ImGuiWindowFlags_HorizontalScrollbar;
	if (m_UseMenuBar)
		flag = ImGuiWindowFlags_MenuBar;
	if (!m_Move)
		flag |= ImGuiWindowFlags_NoMove;


	// 최상위 부모 UI 인 경우
	if (nullptr == m_Parent)
	{
		// Modaless
		if (false == m_Modal)
		{
			ImGui::Begin(m_FullName.c_str(), &bActive, flag);

			if (m_Active != bActive)
			{
				SetActive(bActive);
			}

			Update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->Tick();

				if (m_vecChildUI[i]->m_ChildBorder && i == m_vecChildUI.size() - 1)
					ImGui::Separator();
			}

			ImGui::End();
		}


		// Modal
		else
		{
			ImGui::OpenPopup(m_FullName.c_str());

			if (ImGui::BeginPopupModal(m_FullName.c_str(), &bActive))
			{
				Update();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->Tick();

					if (m_vecChildUI[i]->m_ChildBorder && i == m_vecChildUI.size() - 1)
						ImGui::Separator();
				}

				ImGui::EndPopup();
			}
			else
			{
				if (m_Active != bActive)
				{
					SetActive(bActive);
				}
			}
		}
	}

	// 자식 타입 UI 인 경우
	else
	{
		if (m_ChildBorder)
		{
			ImGui::Separator();
		}

		ImGui::BeginChild(m_FullName.c_str(), m_ChildSize);

		Update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			if (m_vecChildUI[i]->m_ChildBorder)
				ImGui::Separator();

			m_vecChildUI[i]->Tick();

			if (m_vecChildUI[i]->m_ChildBorder && i == m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::EndChild();
	}
}

void DEditor::AddChild(DEditor* _UI)
{
	_UI->m_Parent = this;
	m_vecChildUI.push_back(_UI);
}

void DEditor::SetName(const string& _Name)
{
	m_Name = _Name;

	// ID 문자열 생성
	m_FullName = m_Name;
	char szNum[50] = {};
	_itoa_s(m_ID, szNum, 10);
	m_FullName = m_FullName + "##" + szNum;
}

void DEditor::SetActive(bool _Active)
{
	if (m_Active == _Active)
		return;

	m_Active = _Active;

	if (m_Active)
		Activate();
	else
		Deactivate();
}

void DEditor::SetFocus()
{
	ImGui::SetWindowFocus(m_FullName.c_str());
}
