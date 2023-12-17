#pragma once
#include <Scene.h>


#include <Components/InputComponent.h>

class CollisionTest :
    public Sleepy::Scene
{
public:
    CollisionTest();

    virtual void BeginPlay() override;
    virtual void Update(double deltaTime) override;

    //void MoveDown();
    //void MoveUp();

    void MoveForward();
    void MoveBackward();
    void MoveLeft();
    void MoveRight();
    void ApplyForce();

    void StopMoveForward();
    void StopMoveBackward();
    void StopMoveLeft();
    void StopMoveRight();
    float groundScale = 1.0f;
    float speed = 250.0f;
    glm::vec3 dir;
    Sleepy::Entity* controlledEntity;
    Sleepy::Entity* groundEntity;
    Sleepy::InputComponent* m_Input;
    class btRigidBody* rigidBody;
    class btRigidBody* groundRigidBody;
    //class Sleepy::Camera* camera;
    //class Sleepy::Camera* playerCamera;

};

