#ifndef _STRUCT
#define _STRUCT

// Light Structure
struct tLight
{
    float4 Color; // Light color
    float4 Ambient; // Ambient light
};

struct tLightInfo
{
    tLight light; // Light color and ambient info
    float3 WorldPos; // Light position
    float3 WorldDir; // Light direction
    float Radius; // Light radius
    float Angle; // Light angle
    uint Type; // Light type
    int3 padding; // Padding
};

// Particle Structure
struct tParticle
{
    float4 vColor; // Particle color
    float3 vLocalPos; // Particle local position
    float3 vWorldPos; // Particle world position
    float3 vWorldInitScale; // Initial world scale
    float3 vWorldCurrentScale; // Current world scale
    float3 vWorldRotation; // World rotation

    float3 vForce; // Force applied
    float3 vVelocity; // Velocity

    float NoiseForceAccTime; // Accumulated time for noise force
    float3 NoiseForceDir; // Direction of applied noise force

    float Mass; // Particle mass
    float Life; // Particle lifespan
    float Age; // Particle age
    float NormalizedAge; // Normalized age (Age/Life)
    int Active; // Activation status

    float2 padding; // Padding
};

// Spawn Count Structure
struct tSpawnCount
{
    uint iSpawnCont; // Spawn count
    uint3 padding; // Padding
};

// Particle Module Structure
struct tParticleModule
{
    // Spawn Module
    uint SpawnRate; // Particles spawned per second
    float4 vSpawnColor; // Spawn color
    float4 vSpawnMinScale; // Minimum scale on spawn
    float4 vSpawnMaxScale; // Maximum scale on spawn

    float MinLife; // Minimum life span
    float MaxLife; // Maximum life span

    uint SpawnShape; // 0: Box, 1: Sphere
    float3 SpawnShapeScale; // Scale of the spawn shape (x = radius for Sphere)

    uint BlockSpawnShape; // Block shape: 0: Box, 1: Sphere
    float3 BlockSpawnShapeScale; // Scale for blocked spawn shape

    uint SpaceType; // 0: Local space, 1: World space

    // Spawn Burst Module
    uint SpawnBurstCount; // Number of particles in a burst
    uint SpawnBurstRepeat; // Burst repeat count
    float SpawnBurstRepeatTime; // Burst repeat time interval

    // Add Velocity Module
    uint AddVelocityType; // 0: Random, 1: FromCenter, 2: ToCenter, 4: Fixed
    float3 AddVelocityFixedDir; // Fixed direction for added velocity
    float AddMinSpeed; // Minimum speed for velocity
    float AddMaxSpeed; // Maximum speed for velocity

    // Scale Module
    float StartScale; // Starting scale
    float EndScale; // Ending scale

    // Drag Module
    float DestNormalizedAge; // Target normalized age
    float LimitSpeed; // Speed limit

    // Noise Force Module
    float NoiseForceTerm; // Noise force term
    float NoiseForceScale; // Noise force scale

    // Render Module
    float3 EndColor; // Final color
    uint FadeOut; // Fade out: 0: Off, 1: Based on normalized age
    float StartRatio; // Fade-out effect start (normalized age)
    uint VelocityAlignment; // Velocity alignment: 0: Off, 1: On

    // Module Activation Flags
    int Module[7]; // Activation status for modules
};

#endif
