#pragma once
#include "DGameObject.h"

class DGameObjectEx :
    public DGameObject
{
public:
    CLONE(DGameObjectEx);

public:
    virtual void FinalTick() override;

};
