#pragma once
#include <Scene.h>
#include <Components/TransformComponent.h>
#include <Components/RelationshipComponent.h>
#include <Entity.h>

class SandboxScene :
    public Sleepy::Scene
{
public:
    SandboxScene();

    virtual void BeginPlay() override;
    virtual void Update(double deltaTime) override;

    std::vector<class Boid*> boids;

    int maxBoids = 100;
};

