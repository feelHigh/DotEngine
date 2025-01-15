#pragma once
#include "SE_Sub.h"

class SE_SpriteDisplay
    : public SE_Sub
{
public:
    SE_SpriteDisplay();
    ~SE_SpriteDisplay();

    virtual void Init() override;
    virtual void Update() override;

private:
    void DisplaySprites(); // Helper function to render sprites

};
