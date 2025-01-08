#pragma once
#include "ComponentUI.h"

class TransformUI :
    public ComponentUI
{
public:
    TransformUI();
    ~TransformUI();

public:
    virtual void Update() override;

};
