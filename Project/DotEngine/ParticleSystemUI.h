#pragma once
#include "ComponentUI.h"

class ParticleSystemUI :
    public ComponentUI
{
public:
    ParticleSystemUI();
    ~ParticleSystemUI();

public:
    virtual void Update() override;

};
