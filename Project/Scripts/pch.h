#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine\\DotEngine_D")
#else
#pragma comment(lib, "Engine\\DotEngine")
#endif

#include <Engine\global.h>
#include <Engine\DEngine.h>

#include "DScriptMgr.h"

#endif //PCH_H