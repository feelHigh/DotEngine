#ifndef ENGINE_DMESHRENDER_H
#define ENGINE_DMESHRENDER_H

#include "DRenderComponent.h"

class DMeshRender :
    public DRenderComponent
{
public:
    CLONE(DMeshRender);
    DMeshRender();
    ~DMeshRender();

public:
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

};

#endif // !ENGINE_DMESHRENDER_H