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

    virtual void Init() override;
    virtual void Update(double deltaTime) override;

	
	class Boid* boid;

};

