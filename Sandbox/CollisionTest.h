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
    virtual void EndPlay() override;

    void PhysicsUpdate();
    void UpdateGround(double deltaTime);

    void UpdateControlledEntityVisuals();
    void SpawnCube();
    void DeleteCube();
    void SpawnBall();
    void DeleteBall();
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
    float speed = 200.0f;
    float ballMass = 10.0f;
    bool bAlreadyOverlapped = false;
    glm::vec3 dir;

    Sleepy::Entity* controlledEntity = nullptr;
    Sleepy::Entity* groundEntity = nullptr;

    Sleepy::InputComponent* m_Input = nullptr;
    Sleepy::MeshGroup* planetGroup = nullptr;

    class btRigidBody* ballRigidBody = nullptr;
    class btRigidBody* groundRigidBody = nullptr;

    class btGhostObject* ghostObj = nullptr;
    
    //class Sleepy::Camera* camera;
    //class Sleepy::Camera* playerCamera;

};

