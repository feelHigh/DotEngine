#pragma once
#include "TE_Sub.h"

class TE_TileMapMod
    : public TE_Sub
{
public:
    TE_TileMapMod();
    ~TE_TileMapMod();

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    int m_NumRows;
    int m_NumColumns;

};
