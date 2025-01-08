#include "Global.hlsli"
#include "Func.hlsli"

// Vertex Shader Input
struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

// Vertex Shader Output
struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};

// Vertex Shader for Standard 2D Rendering
VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0;

    // Transform to clip space
    output.vPosition = mul(float4(_in.vPos, 1.0f), matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    // Transform to world space
    output.vWorldPos = mul(float4(_in.vPos, 1.0f), matWorld).xyz;

    return output;
}

// Pixel Shader for Standard 2D Rendering
float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

    // Use flipbook if enabled
    if (UseFlipbook)
    {
        float2 BackGroundLeftTop = LeftTopUV - (BackGroundUV - SliceUV) / 2.0f;
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
        vSpriteUV -= OffsetUV;

        if (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x &&
            LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y)
        {
            vColor = g_AtlasTex.Sample(g_sam_1, vSpriteUV);
        }
        else
        {
            discard;
        }
    }
    else
    {
        // Sample texture if available
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        else
        {
            vColor = float4(1.0f, 0.0f, 1.0f, 1.0f);
        }
    }

    if (vColor.a == 0.0f)
    {
        discard;
    }

    // Apply lighting
    tLight Light = (tLight) 0;

    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }

    vColor.rgb = vColor.rgb * Light.Color.rgb + vColor.rgb * Light.Ambient.rgb;

    return vColor;
}

// Pixel Shader for Alpha Blending
float4 PS_Std2D_Alphablend(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    else
    {
        vColor = float4(1.0f, 0.0f, 1.0f, 1.0f);
    }

    return vColor;
}

// Vertex Shader for Effects
VTX_OUT VS_Effect(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0;

    output.vPosition = mul(float4(_in.vPos, 1.0f), matWVP);
    output.vUV = _in.vUV;

    return output;
}

// Pixel Shader for Effects
float4 PS_Effect(VTX_OUT _in) : SV_Target
{
    if (!g_btex_0)
    {
        discard;
    }

    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    if (vColor.a == 0.0f)
    {
        discard;
    }

    vColor.rgb = g_vec4_0.xyz;

    return vColor;
}
