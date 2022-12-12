#pragma once
#include <Scene.h>
class SandboxScene :
    public Sleepy::Scene
{
public:
    SandboxScene();

    virtual void Init() override;
    virtual void Update() override;
};

