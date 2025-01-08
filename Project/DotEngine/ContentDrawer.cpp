#include "pch.h"
#include "ContentDrawer.h"

// Base Headers
#include "AssetList.h"

// Manager Headers
#include "DEditorMgr.h"
#include "DAssetMgr.h"
#include "DTaskMgr.h"

// Editor Headers
#include "Inspector.h"
#include "TreeUI.h"

ContentDrawer::ContentDrawer()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("ContentDrawerTree");
	AddChild(m_Tree);

	// Ʈ�� �ɼ� ����
	m_Tree->ShowRoot(false);	// ��Ʈ ������ �ʱ�
	m_Tree->UseDrag(true);		// Tree Drag ��� ON
	m_Tree->ShowNameOnly(true); // ����� �̸����� ��� �� Ȯ���ڴ� �����ϰ� �����̸��� ���̱�
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&ContentDrawer::AssetClicked);
	m_Tree->AddPopupDelegate(this, (DELEGATE_1)&ContentDrawer::PopupMenu);
	// Asset ���¸� ContentDrawer �� TreeUI �� �ݿ�
	RenewContent();
}

ContentDrawer::~ContentDrawer()
{
}

void ContentDrawer::Init()
{
	Reload();
}

void ContentDrawer::Update()
{
	// AssetMgr �� �������� �����, Ʈ�� ������ �籸���Ѵ�.
	if (DAssetMgr::GetInst()->IsChanged())
	{
		RenewContent();
	}
}

void ContentDrawer::RenewContent()
{
	// Ʈ���� ������ ���� ����
	m_Tree->Clear();

	// �θ��带 �������� ���� == ��Ʈ��� �Է�
	TreeNode* pRoot = m_Tree->AddNode(nullptr, "Root");

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* pNode = m_Tree->AddNode(pRoot, ToString((ASSET_TYPE)i));
		pNode->SetFrame(true);

		const map<wstring, Ptr<DAsset>>& mapAsset = DAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			m_Tree->AddNode(pNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get());
		}
	}
}

void ContentDrawer::AssetClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;
	if (pNode->IsFrame())
		return;

	Ptr<DAsset> pAsset = (DAsset*)pNode->GetData();

	Inspector* pInspector = (Inspector*)DEditorMgr::GetInst()->FindEditor("Inspector");
	pInspector->SetTargetAsset(pAsset);
	//ImGui::SetWindowFocus(nullptr);
}

void ContentDrawer::Reload()
{
	// ContentDrawer ������ �ִ� �������ϵ��� ��θ� ���� �˾Ƴ���.
	wstring ContentPath = DPathMgr::GetInst()->GetContentPath();
	FindAssetName(ContentPath, L"*.*");

	// �˾Ƴ� ���� ���ϵ��� ��θ� ���ؼ� Asset ���� AssetMgr �� �ε��Ѵ�.
	for (size_t i = 0; i < m_vecAssetPath.size(); ++i)
	{
		LoadAsset(m_vecAssetPath[i]);
	}

	// ���� �Ŵ������� �ε��Ǿ�������, content �������� ���� ������ AssetMgr ���� �����ϱ�
	// �ε��� ���¿� �ش��ϴ� ���� ������ ContentDrawer ������ �ִ��� Exist üũ
	wstring strContentPath = DPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<DAsset>>& mapAsset = DAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		for (const auto pair : mapAsset)
		{
			// �������� ������ ������ ���� ���������� ���⶧���� �Ѿ��.
			if (pair.second->IsEngineAsset())
				continue;

			wstring strRelativePath = pair.second->GetRelativePath();

			if (false == std::filesystem::exists(strContentPath + strRelativePath))
			{
				if (pair.second->GetRefCount() <= 1)
				{
					// ���� ������û
					DTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
				}
				else
				{
					int ret = MessageBox(nullptr, L"�ٸ� ������ �����ǰ� ���� �� �ֽ��ϴ�.\n������ �����Ͻðڽ��ϱ�?", L"���� ���� ����", MB_YESNO);
					if (ret == IDYES)
					{
						// ���� ������û
						DTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
					}
				}
			}
		}
	}
}

void ContentDrawer::FindAssetName(const wstring& _FolderPath, const wstring& _Filter)
{
	WIN32_FIND_DATA tFindData = {};

	// ��ο� �´� ���� �� ������ Ž���� �� �ִ� Ŀ�ο�����Ʈ ����
	wstring strFindPath = _FolderPath + _Filter;
	HANDLE hFinder = FindFirstFile(strFindPath.c_str(), &tFindData);
	assert(hFinder != INVALID_HANDLE_VALUE);

	// Ž�� Ŀ�ο�����Ʈ�� �̿��ؼ� ���� ������ �ݺ��ؼ� ã�Ƴ���
	while (FindNextFile(hFinder, &tFindData))
	{
		wstring strFindName = tFindData.cFileName;

		if (tFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strFindName == L"..")
				continue;

			FindAssetName(_FolderPath + strFindName + L"\\", _Filter);
		}
		else
		{
			wstring RelativePath = DPathMgr::GetInst()->GetRelativePath(_FolderPath + strFindName);
			m_vecAssetPath.push_back(RelativePath);
		}
	}

	FindClose(hFinder);
}

void ContentDrawer::LoadAsset(const path& _Path)
{
	path ext = _Path.extension();

	if (ext == L".mesh")
		DAssetMgr::GetInst()->Load<DMesh>(_Path, _Path);
	else if (ext == L".mtrl")
		DAssetMgr::GetInst()->Load<DMaterial>(_Path, _Path);
	else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || ext == L".bmp" || ext == L".dds" || ext == L".tga"
		|| ext == L".PNG" || ext == L".JPG" || ext == L".JPEG" || ext == L".BMP" || ext == L".DDS" || ext == L".TGA")
		DAssetMgr::GetInst()->Load<DTexture>(_Path, _Path);
	else if (ext == L".sprite")
		DAssetMgr::GetInst()->Load<DSprite>(_Path, _Path);
	else if (ext == L".flip")
		DAssetMgr::GetInst()->Load<DFlipbook>(_Path, _Path);
	else if (ext == L".mp3" || ext == L".mp4" || ext == L".ogg" || ext == L".wav"
		|| ext == L".MP3" || ext == L".MP4" || ext == L".OGG" || ext == L".WAV")
		DAssetMgr::GetInst()->Load<DSound>(_Path, _Path);
	else if (ext == L".pref")
		DAssetMgr::GetInst()->Load<DPrefab>(_Path, _Path);
}

void ContentDrawer::PopupMenu(DWORD_PTR _Param)
{
	TreeNode* pTargetNode = (TreeNode*)_Param;

	Ptr<DAsset> pAsset = (DAsset*)pTargetNode->GetData();

	if (pAsset->GetAssetType() == ASSET_TYPE::PREFAB)
	{
		if (ImGui::MenuItem("Instantiate"))
		{
			Ptr<DPrefab> pPrefab = (DPrefab*)pAsset.Get();

			DGameObject* CloneObj = pPrefab->Instantiate();

			CreateObject(CloneObj, 0);

			ImGui::CloseCurrentPopup();
		}
	}

	//if (ImGui::Button("Close"))
	//ImGui::CloseCurrentPopup();
	ImGui::EndPopup();
}
