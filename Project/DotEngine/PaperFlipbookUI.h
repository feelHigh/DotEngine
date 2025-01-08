#pragma once
#include "ComponentUI.h"

class PaperFlipbookUI :
    public ComponentUI
{
public:
    PaperFlipbookUI();
    ~PaperFlipbookUI();

private:
    virtual void Update() override;

};
