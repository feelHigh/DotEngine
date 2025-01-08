#pragma once
#include "ComponentUI.h"

class Light2DUI :
    public ComponentUI
{
public:
    Light2DUI();
    ~Light2DUI();

public:
    virtual void Update() override;

};
