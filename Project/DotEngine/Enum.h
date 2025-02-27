#pragma once

enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL,
	SPRITE,
	GLOBAL,
	END,
};


enum class ASSET_TYPE
{
	MESH,
	MATERIAL,
	TEXTURE,
	SPRITE,
	FLIPBOOK,
	SOUND,
	PREFAB,
	GRAPHIC_SHADER,
	COMPUTE_SHADER,
	END,
};
extern const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END];
const char* ToString(ASSET_TYPE _Type);


enum class COMPONENT_TYPE
{
	TRANSFORM,
	COLLIDER2D,
	PAPERFLIPBOOK,
	LIGHT2D,
	CAMERA,

	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,

	END,

	SCRIPT,
};
extern const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END];
const char* ToString(COMPONENT_TYPE _Type);


enum DIR
{
	RIGHT,
	UP,
	FRONT,
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,

	GREATER,

	NO_TEST,
	NO_WRITE,
	NO_TEST_NO_WRITE,

	END,
};

enum class BS_TYPE
{
	DEFAULT,
	ALPHABLEND_COVERAGE,
	ALPHABLEND,
	ONE_ONE,
	END,
};


enum SCALAR_PARAM
{
	INT_0, INT_1, INT_2, INT_3,
	FLOAT_0, FLOAT_1, FLOAT_2, FLOAT_3,
	VEC2_0, VEC2_1, VEC2_2, VEC2_3,
	VEC4_0, VEC4_1, VEC4_2, VEC4_3,
	MAT_0, MAT_1, MAT_2, MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};


enum SHADER_DOMAIN
{
	DOMAIN_OPAQUE,		// 불투명
	DOMAIN_MASKED,		// 투명, 불투명
	DOMAIN_TRANSPARENT, // 반투명
	DOMAIN_PARTICLE,	// 파티클
	DOMAIN_EFFECT,		// 2D 이펙트
	DOMAIN_POSTPROCESS, // 후처리
	DOMAIN_UI,			// UI
	DOMAIN_DEBUG,		// 디버그
	DOMAIN_NONE,		// 미설정
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	LINE,
	CUBE,
	SPHERE,
};

enum TASK_TYPE
{
	// Param_0 : Layer, Param_1 : Object Adress
	CREATE_OBJECT,

	// Param_0 : Delete Object Adress
	DELETE_OBJECT,

	// Param_0 : Next Level State
	CHANGE_LEVELSTATE,

	// Param_0 : Level Adress, Param_1 : Level State
	CHANGE_LEVEL,

	// Param_0 : AssetAdress
	DEL_ASSET,

	LEVEL_CHANGED,
	ASSET_CHANGED,

	ADD_CHILD,
	COMPONENT_ACTIVATE,
	COMPONENT_DEACTIVATE,
};

enum LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};


enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class PARTICLE_MODULE
{
	SPAWN,
	SPAWN_BURST,
	ADD_VELOCITY,
	SCALE,
	DRAG,
	NOISE_FORCE,
	RENDER,

	END,
};