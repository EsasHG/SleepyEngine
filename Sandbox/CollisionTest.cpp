#include "CollisionTest.h"

#include <iostream>
#include <Entity.h>
#include <Components/LightComponent.h>
#include <Components/MeshComponent.h>
#include <Components/CameraComponent.h>
#include <TransformSystem.h>
#include <CollisionSystem.h>
#include <Mesh.h>
#include <glm/gtx/norm.hpp>


CollisionTest::CollisionTest()
{
	m_Input = &m_SceneEntity->AddComponent<Sleepy::InputComponent>();
	m_Input->AddKeyBinding(GLFW_KEY_UP, Sleepy::SLE_HELD, std::bind(&CollisionTest::MoveForward, this));
	m_Input->AddKeyBinding(GLFW_KEY_DOWN, Sleepy::SLE_HELD, std::bind(&CollisionTest::MoveBackward, this));
	m_Input->AddKeyBinding(GLFW_KEY_LEFT, Sleepy::SLE_HELD, std::bind(&CollisionTest::MoveLeft, this));
	m_Input->AddKeyBinding(GLFW_KEY_RIGHT, Sleepy::SLE_HELD, std::bind(&CollisionTest::MoveRight, this));

	m_Input->AddKeyBinding(GLFW_KEY_UP, Sleepy::SLE_RELEASED, std::bind(&CollisionTest::StopMoveForward, this));
	m_Input->AddKeyBinding(GLFW_KEY_DOWN, Sleepy::SLE_RELEASED, std::bind(&CollisionTest::StopMoveBackward, this));
	m_Input->AddKeyBinding(GLFW_KEY_LEFT, Sleepy::SLE_RELEASED, std::bind(&CollisionTest::StopMoveLeft, this));
	m_Input->AddKeyBinding(GLFW_KEY_RIGHT, Sleepy::SLE_RELEASED, std::bind(&CollisionTest::StopMoveRight, this));



	std::vector<Sleepy::Vertex> vertices;
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)));
	
	std::vector<Sleepy::Vertex> cubeVertices;

												// positions					// normals									// texture coords
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(  0.0f,  0.0f, -1.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(  0.0f,  0.0f, -1.0f), glm::vec2(  0.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(  0.0f,  0.0f, -1.0f), glm::vec2(  1.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(  0.0f,  0.0f, -1.0f), glm::vec2(  1.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(  0.0f,  0.0f, -1.0f), glm::vec2(  0.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(  0.0f,  0.0f, -1.0f), glm::vec2(  0.0f,  1.0f)));

	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(  0.0f,  0.0f,  1.0f), glm::vec2(  0.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(  0.0f,  0.0f,  1.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(  0.0f,  0.0f,  1.0f), glm::vec2(  1.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(  0.0f,  0.0f,  1.0f), glm::vec2(  1.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(  0.0f,  0.0f,  1.0f), glm::vec2(  0.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(  0.0f,  0.0f,  1.0f), glm::vec2(  0.0f,  0.0f)));
		
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2(  1.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2(  0.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2(  0.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2(  0.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3( -1.0f,  0.0f,  0.0f), glm::vec2(  1.0f,  0.0f)));
		
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(  1.0f,  0.0f,  0.0f), glm::vec2(  1.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(  1.0f,  0.0f,  0.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(  1.0f,  0.0f,  0.0f), glm::vec2(  0.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(  1.0f,  0.0f,  0.0f), glm::vec2(  0.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(  1.0f,  0.0f,  0.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(  1.0f,  0.0f,  0.0f), glm::vec2(  0.0f,  0.0f)));
		
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(  0.0f, -1.0f,  0.0f), glm::vec2(  0.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(  0.0f, -1.0f,  0.0f), glm::vec2(  1.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(  0.0f, -1.0f,  0.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(  0.0f, -1.0f,  0.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(  0.0f, -1.0f,  0.0f), glm::vec2(  0.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(  0.0f, -1.0f,  0.0f), glm::vec2(  0.0f,  1.0f)));
		
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(  0.0f,  1.0f,  0.0f), glm::vec2(  1.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(  0.0f,  1.0f,  0.0f), glm::vec2(  0.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(  0.0f,  1.0f,  0.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(  0.0f,  1.0f,  0.0f), glm::vec2(  1.0f,  0.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(  0.0f,  1.0f,  0.0f), glm::vec2(  0.0f,  1.0f)));
	cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(  0.0f,  1.0f,  0.0f), glm::vec2(  0.0f,  0.0f)));
	

	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f,-1.0f),  glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));  // triangle 2 : begin
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f))); 
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));  // triangle 2 : end
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(1.0f,-1.0f,	 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3 (1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	//cubeVertices.push_back(Sleepy::Vertex(glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));



	m_SceneEntity->bActive = true;

	Sleepy::Material& mat = Sleepy::ModelLibrary::GetInstance().CreateMaterial("color");
	mat.shininess = 2.0f;

	Sleepy::ModelLibrary::GetInstance().AddMesh("quad", vertices);
	Sleepy::ModelLibrary::GetInstance().AddMesh("Cube", cubeVertices);

	Sleepy::Entity& dirLight = CreateEntity("Directional Light");
	dirLight.AddComponent<Sleepy::DirLightComponent>(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(-0.2f, -0.6f, -0.3f));

	Sleepy::Entity& pointLight = CreateEntity("Point Light");
	pointLight.AddComponent<Sleepy::PointLightComponent>(glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	pointLight.SetPosition(glm::vec3(0.0f, 0.0f, 0.5f));

	Sleepy::MeshGroup* planetGroup = Sleepy::ModelLibrary::GetInstance().AddMesh("Assets\\planet\\planet.obj");

	if (planetGroup)
	{
		Sleepy::Entity& planetEntity = LoadMeshGroup(planetGroup, *m_SceneEntity, "Planet");
		planetEntity.SetPosition(glm::vec3(20.0f, 5.0f, 0.0f));
		controlledEntity = &planetEntity;
	}

	std::vector<std::string> faces
	{
		"Assets\\skybox\\right.jpg",
		"Assets\\skybox\\left.jpg",
		"Assets\\skybox\\top.jpg",
		"Assets\\skybox\\bottom.jpg",
		"Assets\\skybox\\front.jpg",
		"Assets\\skybox\\back.jpg"
	};
	std::string face = "Assets\\skybox2.png";

	Sleepy::ModelLibrary::GetInstance().LoadCubemapTexture(faces);



	btCollisionShape* colShape = Sleepy::CollisionSystem::GetInstance().GetSphereCollisionShape();
	btCollisionShape* groundColShape = new btBoxShape(btVector3(50.0f, 50.0f, 50.0f));
	Sleepy::CollisionSystem::GetInstance().m_collisionShapes.push_back(groundColShape);
	assert((!groundColShape || groundColShape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	btScalar mass(0.0f);
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	groundEntity = &CreateEntity("ground");
	groundEntity->AddComponent<Sleepy::MeshComponent>("Cube", "color");
	groundEntity->SetScale(glm::vec3(50.0f, 50.0f, 50.0f));
	groundEntity->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	groundEntity->SetPosition(glm::vec3(0.0f, -51.0f, 0.0f));
	
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundColShape->calculateLocalInertia(mass, localInertia);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -51, 0));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, groundColShape, localInertia);
	
	groundRigidBody = new btRigidBody(cInfo);
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	groundRigidBody->setFriction(btScalar(0.5f));
	groundRigidBody->setRollingFriction(btScalar(0.5f));
	//body->setSpinningFriction(btScalar(0.1f));
	groundRigidBody->setUserIndex(-1);
	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->addRigidBody(groundRigidBody);


	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	mass = 100.f;
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	isDynamic = (mass != 0.f);

	localInertia = btVector3(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(20.0f, 5.0f, 0));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	
	rigidBody = new btRigidBody(rbInfo);
	//rigidBody->applyCentralForce(btVector3(0.0f, 0.0f, 0.5f));
	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->addRigidBody(rigidBody);

	rigidBody->setGravity(btVector3(0.0f, -9.8f, 0.0f));

	Sleepy::Entity& gameCamera = CreateGameObject<Sleepy::Entity>("Game Camera");
	Sleepy::CameraComponent& cam = gameCamera.AddComponent<Sleepy::CameraComponent>();
	gameCamera.SetPosition(glm::vec3(0.0f, 180.0f, 110.0f));/*
	gameCamera.SetRotation(glm::vec3( - 90.0f, -60.0f, 0.0f));*/
	cam.bPossessOnStart = true;
	cam.lookatTarget = groundEntity;

}



void CollisionTest::BeginPlay()
{
	Sleepy::Scene::BeginPlay();
	//btVector3 intertia;
	//rigidBody->getCollisionShape()->calculateLocalInertia(1.0f,intertia);
	//rigidBody->setMassProps(1.0f, intertia);
	//rigidBody->applyCentralForce(btVector3(10000.0f, 0.0f, 0.0f));
	btTransform initialTransform;
	glm::vec3 pos = controlledEntity->GetPosition();
	initialTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	//rigidBody->applyTorque(btVector3(0.f, 0.001f, 0.0f));

	rigidBody->setWorldTransform(initialTransform);
	rigidBody->getMotionState()->setWorldTransform(initialTransform);

}

void CollisionTest::Update(double deltaTime)
{
	Sleepy::Scene::Update(deltaTime);
	ApplyForce();
	//rigidBody->setLinearVelocity(btVector3(5.0f, 0.0f, 0.0f));
	//rigidBody->applyCentralForce(btVector3(1.0f, 0.0f, 0.0f));

	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);
	btVector3 btPos = trans.getOrigin();
	btQuaternion btQuat = trans.getRotation();
	controlledEntity->SetRotation(glm::quat(btQuat.getW(), btQuat.getX(), btQuat.getY(), btQuat.getZ()));

	controlledEntity->SetPosition(glm::vec3(btPos.x(), btPos.y(), btPos.z()));

	groundScale = groundScale -(0.05 * deltaTime);
	if (groundScale <= 0)
		groundScale = 0;
	//groundRigidBody->getCollisionShape()->setLocalScaling(btVector3(groundScale, groundScale, groundScale));
	groundRigidBody->getCollisionShape()->setLocalScaling(btVector3(groundScale, groundScale, groundScale));
	btVector3 s = groundRigidBody->getCollisionShape()->getLocalScaling();
	//groundEntity->SetScale(glm::vec3(50.0f * groundScale, 50.0f * groundScale, 50.0f * groundScale));
	groundEntity->SetScale(glm::vec3(50.0f * s.x(), 50.0f * s.z() , 50.0f * s.y()));
	glm::vec3 visScale = groundEntity->GetScale();

	btTransform transform;
	btQuaternion quat;
	quat.setEuler(0.0f, 0.0f, glm::radians(180.0f) * (1 - groundScale));
	float eulerZ, eulerY, eulerX;
	quat.getEulerZYX(eulerZ, eulerY, eulerX);

	transform.setOrigin(btVector3(0.0f, -51.0f, 0.0f));
	transform.setRotation(quat);
	quat = transform.getRotation();
	quat.getEulerZYX(eulerZ, eulerY, eulerX);


	groundRigidBody->setWorldTransform(transform);
	groundRigidBody->getMotionState()->setWorldTransform(transform);
	
	btTransform newTrans;
	groundRigidBody->getMotionState()->getWorldTransform(newTrans);

	btQuaternion newQuat = newTrans.getRotation();

	newQuat.getEulerZYX(eulerZ, eulerY, eulerX);
	groundEntity->SetRotation(glm::vec3(glm::degrees(eulerX), glm::degrees(eulerY), glm::degrees(eulerZ)));



	glm::vec3 newRot = groundEntity->GetRotation();
}



void CollisionTest::MoveForward()
{
	dir.z = -1;
}

void CollisionTest::MoveBackward()
{
	dir.z = 1;
}

void CollisionTest::MoveLeft()
{
	dir.x = -1;

}

void CollisionTest::MoveRight()
{
	dir.x = 1;
}

void CollisionTest::ApplyForce()
{
	if (dir.x != 0 || dir.z != 0)
	{

		glm::vec3 dirNorm = glm::normalize(dir);
		glm::vec3 dirSpeed = dirNorm * speed;
		glm::vec3 forcePoint = (-dirNorm)*3.6f;
		//	rigidBody->setLinearVelocity(btVector3(dirNorm.x, dirNorm.y, dirNorm.z));
		//rigidBody->applyForce(btVector3(dirSpeed.x, dirSpeed.y, dirSpeed.z), btVector3(forcePoint.x, forcePoint.y, forcePoint.z));
		rigidBody->applyCentralForce(btVector3(dirSpeed.x, dirSpeed.y, dirSpeed.z));
		rigidBody->applyTorque(btVector3(dirSpeed.z, dirSpeed.y, -dirSpeed.x));
		//rigidBody->apply
		//rigidBody->applyForce(btVector3(dirNorm.x, dirNorm.y, dirNorm.z), btVector3(0.0f, 1.0f, 0.0f));
	}
}

void CollisionTest::StopMoveForward()
{
	dir.z = 0;

	//if (dir.x == 0)
	//{
	//	rigidBody->setLinearVelocity(btVector3(0.0f,0.0f,0.0f));
	//}
	//else
	//{
	//	glm::vec3 dirNorm = glm::normalize(dir) * speed;
	//	rigidBody->setLinearVelocity(btVector3(dirNorm.x, dirNorm.y, dirNorm.z));
	//}


}

void CollisionTest::StopMoveBackward()
{
	dir.z = 0;
	
	//if (dir.x == 0)
	//{
	//	rigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	//}
	//else
	//{
	//	glm::vec3 dirNorm = glm::normalize(dir) * speed;
	//	rigidBody->setLinearVelocity(btVector3(dirNorm.x, dirNorm.y, dirNorm.z));
	//}
}

void CollisionTest::StopMoveLeft()
{
	dir.x = 0;

	//if (dir.z == 0)
	//{
	//	rigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	//}
	//else
	//{
	//	glm::vec3 dirNorm = glm::normalize(dir) * speed;
	//	rigidBody->setLinearVelocity(btVector3(dirNorm.x, dirNorm.y, dirNorm.z));
	//}

}

void CollisionTest::StopMoveRight()
{
	dir.x = 0;


	/*if (dir.z == 0)
	{
		rigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		glm::vec3 dirNorm = glm::normalize(dir) * speed;
		rigidBody->setLinearVelocity(btVector3(dirNorm.x, dirNorm.y, dirNorm.z));
	}*/

}
