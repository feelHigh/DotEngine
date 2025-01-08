#pragma once

#define SINGLE(Type) private:\
						Type();\
						~Type();\
						friend class DSingleton<Type>;

#define DEVICE 	DDevice::GetInst()->GetDevice()
#define CONTEXT DDevice::GetInst()->GetContext()
#define MAX_LAYER 32

#define DT DTimeMgr::GetInst()->GetDeltaTime()
#define EngineDT DTimeMgr::GetInst()->GetEngineDeltaTime()

#define KEY_CHECK(KEY, STATE) DKeyMgr::GetInst()->GetKeyState(KEY) == STATE
#define KEY_TAP(KEY) DKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::TAP
#define KEY_PRESSED(KEY) DKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::PRESSED
#define KEY_RELEASED(KEY) DKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::RELEASED

#define CLONE(Type) virtual Type* Clone(){return new Type(*this); }
#define CLONE_DISABLE(Type) virtual Type* Clone() { return nullptr; }

#define DELETE(p) if(nullptr != p) delete p; p = nullptr;


typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;