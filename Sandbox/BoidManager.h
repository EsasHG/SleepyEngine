#pragma once
#include <Entity.h>
#include "Boid.h"

class BoidManager :
    public Sleepy::Entity
{
public:
    BoidManager(std::string name, class Sleepy::SceneBase* scene)
        : Sleepy::Entity(name, scene)
    {
        enableUpdate = true;
    }
    virtual void BeginPlay() override;
    virtual void Update(double deltaTime) override;
private:
    bool showManagerWindow = true;

    std::vector<class Boid*> boids;
    struct BoidInfo info;
    int maxBoids = 300;
};

