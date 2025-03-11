#ifndef ENGINE_KEYMGR_H
#define ENGINE_KEYMGR_H

#include "Singleton.h"

enum class KEY
{
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,

	LEFT, RIGHT, UP, DOWN,

	ENTER,
	SPACE,
	ALT,
	CTRL,
	LSHIFT,
	ESC,

	LBTN,
	RBTN,

	KEY_END,
};

enum class KEY_STATE
{
	TAP,
	PRESSED,
	RELEASED,
	NONE,
};


struct tKeyInfo
{
	KEY			eKey;		// Type
	KEY_STATE	eKeyState;	// State
	bool		bPressed;	// If pressed
};

class DKeyMgr
	: public DSingleton<DKeyMgr>
{
	SINGLE(DKeyMgr);
public:
	void Init();
	void Tick();

public:
	KEY_STATE GetKeyState(KEY _Key) { return m_vecKeyInfo[(UINT)_Key].eKeyState; }
	Vec2 GetMousePos() { return m_MousePos; }
	Vec2 GetDragDir() { return m_DragDir; }

	void MouseCapture(bool _bCapture)
	{
		//m_MouseCapture = _bCapture;		
		//m_CapturePos = m_MousePos;
	}

private:
	vector<tKeyInfo>	m_vecKeyInfo;

	Vec2				m_MousePos;
	Vec2				m_PrevMousePos;
	Vec2				m_DragDir;

	Vec2				m_CapturePos;
	bool				m_MouseCapture;

};

#endif // !ENGINE_KEYMGR_H