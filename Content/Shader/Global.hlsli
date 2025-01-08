#ifndef _GLOBAL
#define _GLOBAL

#include "Struct.hlsli"

#define PI 3.1415926535f

// Object transformation matrices
cbuffer OBJECT_POS : register(b0)
{
    row_major float4x4 matWorld;
    row_major float4x4 matView;
    row_major float4x4 matProj;

    row_major float4x4 matWV;
    row_major float4x4 matWVP;
};

// Material properties
cbuffer MATERIAL : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;

    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;

    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;

    row_major float4x4 g_mat_0;
    row_major float4x4 g_mat_1;
    row_major float4x4 g_mat_2;
    row_major float4x4 g_mat_3;

    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btexcube_0;
    int g_btexcube_1;
    int g_btexarr_0;
    int g_btexarr_1;

    int2 mtrlpadding;
};

// Sprite information
cbuffer SPRITE_INFO : register(b2)
{
    float2 LeftTopUV;
    float2 SliceUV;
    float2 BackGroundUV;
    float2 OffsetUV;
    int UseFlipbook;
    int3 SpriteInfoPadding;
};

// Global data
cbuffer GLOBAL_DATA : register(b3)
{
    // Timing information
    float g_DT;
    float g_EngineDT;
    float g_Time;
    float g_EngineTime;

    // Render target resolution
    float2 g_Resolution;

    // Number of lights in structured buffers
    int g_Light2DCount;
    int g_Light3DCount;
}

// Samplers
SamplerState g_sam_0 : register(s0); // Anisotropic filter sampler
SamplerState g_sam_1 : register(s1); // Point filter sampler
SamplerState g_sam_2 : register(s2); // Linear filter sampler

// Textures
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
TextureCube g_texcube_0 : register(t6);
TextureCube g_texcube_1 : register(t7);
Texture2DArray g_texarr_0 : register(t8);
Texture2DArray g_texarr_1 : register(t9);

Texture2D g_AtlasTex : register(t10);

// Structured buffers
StructuredBuffer<tLightInfo> g_Light2D : register(t11);
StructuredBuffer<tLightInfo> g_Light3D : register(t12);

#endif
