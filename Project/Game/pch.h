#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

// Engine
#ifdef _DEBUG
#pragma comment(lib, "Engine\\DotEngine_D")
#else
#pragma comment(lib, "Engine\\DotEngine")
#endif

// Scripts
#ifdef _DEBUG
#pragma comment(lib, "Scripts\\Scripts_D")
#else
#pragma comment(lib, "Scripts\\Scripts")
#endif

#include <Engine\Global.h>
#include <Engine\DEngine.h>

#include <set>
using std::set;

#endif //PCH_H