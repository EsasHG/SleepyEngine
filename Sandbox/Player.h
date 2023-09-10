#pragma once
#include <Entity.h>
#include <Components/MeshComponent.h>
#include <Components/InputComponent.h>

class Player :
    public Sleepy::Entity
{
	public:
	Player(std::string name, class Sleepy::SceneBase* scene)
		: Sleepy::Entity(name, scene), mesh(AddComponent<Sleepy::MeshComponent>("planet_0", "color"))
	{
		enableUpdate = true;
		bActive = true;
	}
	//~Player();
	virtual void BeginPlay() override;
	virtual void Update(double deltaTime) override;

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void CursorMoveCallback(double xPos, double yPos);

	Sleepy::MeshComponent& mesh;

	bool inGroup1 = false;
	bool inGroup2 = false;
	glm::vec3 velocity = glm::vec3(0.0);
	float maxSpeed = 50.0f;
	Sleepy::InputComponent* m_Input;
	class Sleepy::CameraComponent* m_Camera;
private:
	bool firstMouse;
	double lastX = 0, lastY = 0;
	double xoffset = 0, yoffset = 0;
};

