#include "pch.h"

tTransform g_Trans = {};
tGlobalData g_GlobalData = {};

const char* ASSET_TYPE_STRING[] =
{
	"MESH",
	"MATERIAL",
	"TEXTURE",
	"SPRITE",
	"FLIPBOOK",
	"SOUND",
	"PREFAB",
	"GRAPHIC_SHADER",
	"COMPUTE_SHADER",
};

const char* ToString(ASSET_TYPE _Type) { return ASSET_TYPE_STRING[(UINT)_Type]; }

const char* COMPONENT_TYPE_STRING[] =
{
	"TRANSFORM",
	"COLLIDER2D",
	"PAPERFLIPBOOK",
	"LIGHT2D",
	"CAMERA",
	"MESHRENDER",
	"TILEMAP",
	"PARTICLE_SYSTEM",
};

const char* ToString(COMPONENT_TYPE _Type) { return COMPONENT_TYPE_STRING[(UINT)_Type]; }
