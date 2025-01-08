#ifndef _FUNC
#define _FUNC

#include "Global.hlsli"
#include "Struct.hlsli"

// Check if a texture is bound
int IsBinding(Texture2D _tex)
{
    int width = 0, height = 0;
    _tex.GetDimensions(width, height);
    
    if (0 == width || height == 0)
        return false;
    
    return true;
}

// Calculate light contribution for 2D lights
void CalculateLight2D(int _LightIdx, float3 _WorldPos, inout tLight _Light)
{
    tLightInfo Info = g_Light2D[_LightIdx];
    
    // Directional Light
    if (0 == Info.Type)
    {
        _Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }
    
    // Point Light
    else if (1 == Info.Type)
    {
        // Distance from light to pixel
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);
        
        // Intensity based on distance, modeled with a cosine function
        float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));
        
        // Final color contribution
        _Light.Color.rgb += Info.light.Color.rgb * fPow;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }
    
    // Spot Light
    else if (2 == Info.Type)
    {
        // Vector from light to pixel
        float3 toPixel = _WorldPos - Info.WorldPos;
        float dist = length(toPixel.xy);

        // Normalize direction vectors
        float3 lightDir = normalize(Info.WorldDir);
        float3 pixelDir = normalize(toPixel);

        // Calculate the angle between the light direction and the pixel direction
        float spotAngleCosine = dot(lightDir, pixelDir);
        float angleFalloff = saturate((spotAngleCosine - cos(Info.Angle)) / (1.0f - cos(Info.Angle)));

        // Distance attenuation
        float distanceFalloff = saturate(1.0f - dist / Info.Radius);

        // Final intensity combining angle and distance falloff
        float intensity = angleFalloff * distanceFalloff;

        // Add light contribution
        _Light.Color.rgb += Info.light.Color.rgb * intensity;
        _Light.Ambient.rgb += Info.light.Ambient.rgb * intensity;
    }
}

// Generate random vector using a noise texture
float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
{
    float2 vUV = (float2) 0.f;
    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_EngineTime * 0.5f;
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_EngineTime * 0.1f;
    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_1, vUV, 0).xyz;
    
    return vRandom;
}

#endif
