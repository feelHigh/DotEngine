#include "Global.hlsli"
#include "Func.hlsli"

// Input structure for vertex shader
struct VTX_IN
{
    float3 vPos : POSITION; // Vertex position
    float4 vColor : COLOR; // Vertex color
    float2 vUV : TEXCOORD; // UV coordinates
};

// Output structure for vertex shader
struct VTX_OUT
{
    float4 vPosition : SV_Position; // Transformed position (clip space)
    float4 vColor : COLOR; // Vertex color
    float2 vUV : TEXCOORD; // UV coordinates
};

// Vertex Shader for standard 2D rendering
VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output;

    // Transform vertex position to clip space using World-View-Projection matrix
    output.vPosition = mul(float4(_in.vPos, 1.0f), matWVP);

    // Pass through vertex color and UV coordinates
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    return output;
}

// Pixel Shader for standard 2D rendering
float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vColor;

    // Check if a texture is bound (g_btex_0 is a scalar parameter set in the material)
    if (g_btex_0)
    {
        // Sample the texture using UV coordinates
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    else
    {
        // Fallback to magenta if no texture is bound
        vColor = float4(1.0f, 0.0f, 1.0f, 1.0f);
    }

    // Handle transparency: discard pixels with zero alpha
    if (vColor.a == 0.0f)
    {
        discard;
    }

    return vColor;
}
