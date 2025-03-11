#include "pch.h"
#include "DKeyMgr.h"

// Engine Headers
#include "DEngine.h"

UINT g_RealKey[(UINT)KEY::KEY_END] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',

	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,

	VK_RETURN,
	VK_SPACE,
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,
};

DKeyMgr::DKeyMgr()
{
}

DKeyMgr::~DKeyMgr()
{
}

void DKeyMgr::Init()
{
	for (int i = 0; i < (int)KEY::KEY_END; ++i)
	{
		tKeyInfo info = {};
		info.eKey = (KEY)i;
		info.eKeyState = KEY_STATE::NONE;
		info.bPressed = false;

		m_vecKeyInfo.push_back(info);
	}
}

void DKeyMgr::Tick()
{
	// MainWindow is in a state of focus
	if (DEngine::GetInst()->GetMainWnd() == GetFocus())
	{
		for (UINT i = 0; i < (UINT)m_vecKeyInfo.size(); ++i)
		{
			// Check if the key is pressed right now
			if (0x8001 & GetAsyncKeyState(g_RealKey[(UINT)m_vecKeyInfo[i].eKey]))
			{
				// If pressed before
				if (m_vecKeyInfo[i].bPressed)
				{
					m_vecKeyInfo[i].eKeyState = KEY_STATE::PRESSED;
				}
				// If not pressed before and pressed now
				else
				{
					m_vecKeyInfo[i].eKeyState = KEY_STATE::TAP;
				}

				m_vecKeyInfo[i].bPressed = true;
			}

			// The key is not pressed
			else
			{
				// If pressed before
				if (m_vecKeyInfo[i].bPressed)
				{
					m_vecKeyInfo[i].eKeyState = KEY_STATE::RELEASED;
				}

				// If not pressed before and pressed now
				else
				{
					m_vecKeyInfo[i].eKeyState = KEY_STATE::NONE;
				}

				m_vecKeyInfo[i].bPressed = false;
			}
		}

		// Calculate mouse coordinates
		if (m_MouseCapture)
		{
			POINT ptMousePos = { };
			GetCursorPos(&ptMousePos);
			ScreenToClient(DEngine::GetInst()->GetMainWnd(), &ptMousePos);
			m_MousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);

			m_DragDir = m_MousePos - m_CapturePos;

			POINT ptCapturePos = { (int)m_CapturePos.x, (int)m_CapturePos.y };
			ClientToScreen(DEngine::GetInst()->GetMainWnd(), &ptCapturePos);
			SetCursorPos(ptCapturePos.x, ptCapturePos.y);
		}
		else
		{
			m_PrevMousePos = m_MousePos;
			POINT ptMousePos = { };
			GetCursorPos(&ptMousePos);
			ScreenToClient(DEngine::GetInst()->GetMainWnd(), &ptMousePos);
			m_MousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);
			m_DragDir = m_MousePos - m_PrevMousePos;
		}

	}

	// Windows is off-focusing
	else
	{
		for (UINT i = 0; i < (UINT)m_vecKeyInfo.size(); ++i)
		{
			if (m_vecKeyInfo[i].eKeyState == KEY_STATE::TAP
				|| m_vecKeyInfo[i].eKeyState == KEY_STATE::PRESSED)
			{
				m_vecKeyInfo[i].eKeyState = KEY_STATE::RELEASED;
			}
			else
			{
				m_vecKeyInfo[i].eKeyState = KEY_STATE::NONE;
			}

			m_vecKeyInfo[i].bPressed = false;
		}
	}
}
