#pragma once
#include <Entity.h>
#include <Components/MeshComponent.h>

class Boid : public Sleepy::Entity
{
public:
	Boid(std::string name, class Sleepy::SceneBase* scene) : Sleepy::Entity(name, scene), mesh(AddComponent<Sleepy::MeshComponent>("quad", "color", "color"))
	{
		enableUpdate = true;
	}

	virtual void Update(double deltaTime) override;
	Sleepy::MeshComponent& mesh;
};

