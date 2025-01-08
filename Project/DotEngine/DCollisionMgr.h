#pragma once
#include "Singleton.h"

class DCollider2D;

union COLLIDER_ID
{
	struct {
		DWORD LeftID;
		DWORD RightID;
	};
	ULONGLONG ID;
};

class DCollisionMgr :
	public DSingleton<DCollisionMgr>
{
	SINGLE(DCollisionMgr);
public:
	void Tick();

public:
	void CollisionCheck(UINT _Layer1, UINT _Layer2);
	void CollisionCheckClear() { memset(m_Matrix, 0, sizeof(UINT) * MAX_LAYER); }

private:
	void CollisionBtwLayer(UINT _Left, UINT _Right);
	bool IsCollision(DCollider2D* _Left, DCollider2D* _Right);

private:
	UINT					m_Matrix[MAX_LAYER];
	map<ULONGLONG, bool>	m_mapCollisionInfo;

};
