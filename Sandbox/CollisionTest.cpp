#include "CollisionTest.h"

#include <iostream>
#include <Entity.h>
#include <Components/LightComponent.h>
#include <Components/MeshComponent.h>
#include <Components/CameraComponent.h>
#include <TransformSystem.h>
#include <CollisionSystem.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

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
	
	m_SceneEntity->bActive = true;

	Sleepy::Material& mat = Sleepy::ModelLibrary::GetInstance().CreateMaterial("color");
	mat.shininess = 2.0f;

	Sleepy::Entity& dirLight = CreateEntity("Directional Light");
	dirLight.AddComponent<Sleepy::DirLightComponent>(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(-0.2f, -0.6f, -0.3f));

	Sleepy::Entity& pointLight = CreateEntity("Point Light");
	pointLight.AddComponent<Sleepy::PointLightComponent>(glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	pointLight.SetPosition(glm::vec3(0.0f, 0.0f, 0.5f));

	planetGroup = Sleepy::ModelLibrary::GetInstance().AddMesh("Assets\\planet\\planet.obj");
	
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



	//create trigger
	btGhostPairCallback* ghostCall = new btGhostPairCallback();
	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(ghostCall);

	ghostObj = new btGhostObject();
	btCollisionShape* shape = new btBoxShape(btVector3(50, 5, 50));
	ghostObj->setCollisionShape(shape);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0, -35, 0));
	ghostObj->setWorldTransform(trans);
	ghostObj->setCollisionFlags(ghostObj->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->addCollisionObject(ghostObj, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);

	//create game camera
	Sleepy::Entity& gameCamera = CreateGameObject<Sleepy::Entity>("Game Camera");
	Sleepy::CameraComponent& cam = gameCamera.AddComponent<Sleepy::CameraComponent>();
	gameCamera.SetPosition(glm::vec3(0.0f, 14.0f, 30.0f));
	gameCamera.SetRotation(glm::vec3( - 90.0f, -30.0f, 0.0f));
	gameCamera.bActive = true;
	cam.UpdateVectors();
	cam.bPossessOnStart = true;

}


void CollisionTest::BeginPlay()
{
	Sleepy::Scene::BeginPlay();

	//SpawnCube();

	//SpawnBall();

	if (controlledEntity)
	{
		btTransform initialTransform;
		glm::vec3 pos = controlledEntity->GetPosition();
		initialTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

		ballRigidBody->setWorldTransform(initialTransform);
		ballRigidBody->getMotionState()->setWorldTransform(initialTransform);
	}

}

void CollisionTest::Update(double deltaTime)
{
	Sleepy::Scene::Update(deltaTime);

	if (groundEntity)
		UpdateGround(deltaTime);
	else
		SpawnCube();

	if (controlledEntity)
	{
		ApplyForce();
		UpdateControlledEntityVisuals();
	}
	else
	{
		SpawnBall();
	}

	int overlappingObjects = ghostObj->getNumOverlappingObjects();

	std::cout << "Overlapping objects:" + std::to_string(overlappingObjects) << std::endl;
	if (overlappingObjects == 0)
		bAlreadyOverlapped = false;

	for (int i = 0; i < ghostObj->getNumOverlappingObjects(); i++)
	{
		btCollisionObject* obj = ghostObj->getOverlappingObject(i);

		Sleepy::Entity* ent = (Sleepy::Entity*)(obj->getUserPointer());
		if (!bAlreadyOverlapped && ent && controlledEntity && (ent->GetHandle() == controlledEntity->GetHandle()))
		{
			DeleteBall();
			SpawnBall();
			//Teleport ball 
			/*btVector3 zeroVector(0, 0, 0);
			ballRigidBody->clearForces();
			ballRigidBody->setLinearVelocity(zeroVector);
			ballRigidBody->setAngularVelocity(zeroVector);


			btTransform trans;
			trans.setIdentity();
			trans.setOrigin(btVector3(0.0f, 2.0f, 0.0f));
			ballRigidBody->setWorldTransform(trans);
			ballRigidBody->getMotionState()->setWorldTransform(trans);

			controlledEntity->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));*/

			bAlreadyOverlapped = true;


			//groundScale = 1.0f;

			//groundRigidBody->getCollisionShape()->setLocalScaling(btVector3(groundScale, groundScale, groundScale));
			//btVector3 s = groundRigidBody->getCollisionShape()->getLocalScaling();
			//groundEntity->SetScale(glm::vec3(5.0f * s.x(), 5.0f * s.z(), 5.0f * s.y()));
			//glm::vec3 visScale = groundEntity->GetScale();

			//btTransform groundTransform;
			//btQuaternion quat;
			//quat.setEuler(0.0f, 0.0f, glm::radians(180.0f));
			//float eulerZ, eulerY, eulerX;
			//quat.getEulerZYX(eulerZ, eulerY, eulerX);

			//groundTransform.setOrigin(btVector3(0.0f, -5.1f, 0.0f));
			//groundTransform.setRotation(quat);
			//quat = groundTransform.getRotation();
			//quat.getEulerZYX(eulerZ, eulerY, eulerX);

			//groundRigidBody->setWorldTransform(groundTransform);
			//groundRigidBody->getMotionState()->setWorldTransform(groundTransform);

			//btTransform newTrans;
			//groundRigidBody->getMotionState()->getWorldTransform(newTrans);
			//btQuaternion newQuat = newTrans.getRotation();

			//newQuat.getEulerZYX(eulerZ, eulerY, eulerX);
			//groundEntity->SetRotation(glm::vec3(glm::degrees(eulerX), glm::degrees(eulerY), glm::degrees(eulerZ)));

			//Sleepy::CollisionSystem::GetInstance().dynamicsWorld->updateAabbs();
			//Sleepy::CollisionSystem::GetInstance().dynamicsWorld->setInternalTickCallback(&CollisionTest::PhysicsUpdate);
		}
	}
}

void CollisionTest::EndPlay()
{
	Scene::EndPlay();
	controlledEntity = nullptr;
	ballRigidBody->setUserPointer(nullptr);

	groundEntity = nullptr;
	groundRigidBody->setUserPointer(nullptr);

}

void CollisionTest::PhysicsUpdate()
{
}


void CollisionTest::UpdateGround(double deltaTime)
{
	//groundScale = groundScale - (0.05f * deltaTime);
	groundScale = groundScale - (0.0005f);
	if (groundScale <= 0.1f)
	{
		groundScale = 1.0f;
		DeleteCube();
		SpawnCube();
	}
	//groundRigidBody->getCollisionShape()->setLocalScaling(btVector3(groundScale, groundScale, groundScale));
	groundRigidBody->getCollisionShape()->setLocalScaling(btVector3(groundScale, groundScale, groundScale));
	btVector3 s = groundRigidBody->getCollisionShape()->getLocalScaling();
	//groundEntity->SetScale(glm::vec3(50.0f * groundScale, 50.0f * groundScale, 50.0f * groundScale));
	groundEntity->SetScale(glm::vec3(5.0f * s.x(), 5.0f * s.z(), 5.0f * s.y()));
	glm::vec3 visScale = groundEntity->GetScale();

	btTransform groundTransform;
	btQuaternion quat;
	quat.setEuler(0.0f, 0.0f, glm::radians(180.0f) * (1.0f - groundScale));
	float eulerZ, eulerY, eulerX;
	quat.getEulerZYX(eulerZ, eulerY, eulerX);

	groundTransform.setOrigin(btVector3(0.0f, -5.1f, 0.0f));
	groundTransform.setRotation(quat);
	quat = groundTransform.getRotation();
	quat.getEulerZYX(eulerZ, eulerY, eulerX);

	groundRigidBody->setWorldTransform(groundTransform);
	groundRigidBody->getMotionState()->setWorldTransform(groundTransform);

	btTransform newTrans;
	groundRigidBody->getMotionState()->getWorldTransform(newTrans);
	btQuaternion newQuat = newTrans.getRotation();

	newQuat.getEulerZYX(eulerZ, eulerY, eulerX);
	groundEntity->SetRotation(glm::vec3(glm::degrees(eulerX), glm::degrees(eulerY), glm::degrees(eulerZ)));
}

void CollisionTest::UpdateControlledEntityVisuals()
{
	btTransform trans;
	ballRigidBody->getMotionState()->getWorldTransform(trans);
	btVector3 btPos = trans.getOrigin();
	btQuaternion btQuat = trans.getRotation();
	controlledEntity->SetRotation(glm::quat(btQuat.getW(), btQuat.getX(), btQuat.getY(), btQuat.getZ()));
	controlledEntity->SetPosition(glm::vec3(btPos.x(), btPos.y(), btPos.z()));
}

void CollisionTest::SpawnBall()
{
	controlledEntity = &LoadMeshGroup(planetGroup, *m_SceneEntity, "Planet");

	controlledEntity->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	controlledEntity->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	/// Create Dynamic Objects
	btCollisionShape* colShape = new btSphereShape(btScalar(0.35f));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (ballMass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(ballMass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0f, 2.0f, 0));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btMotionState* ballMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(ballMass, ballMotionState, colShape, localInertia);

	ballRigidBody = new btRigidBody(rbInfo);
	ballRigidBody->setUserPointer(controlledEntity);

	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->addRigidBody(ballRigidBody);

	ballRigidBody->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	ballRigidBody->setWorldTransform(startTransform);
	ballRigidBody->getMotionState()->setWorldTransform(startTransform);


	if (Sleepy::Application::s_Playing)
	{
		controlledEntity->bActive = true;
		controlledEntity->BeginPlay();
	}

}

void CollisionTest::DeleteBall()
{
	delete ballRigidBody->getMotionState();

	btCollisionShape* ballColShape = ballRigidBody->getCollisionShape();

	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->removeRigidBody(ballRigidBody);
	delete ballRigidBody;
	ballRigidBody = nullptr;

	delete ballColShape;

	DeleteEntity(*controlledEntity);
	controlledEntity = nullptr;
}

void CollisionTest::SpawnCube()
{
	groundScale = 1.0f;
	groundEntity = &CreateEntity("ground");
	groundEntity->AddComponent<Sleepy::MeshComponent>("Cube", "color");
	groundEntity->SetPosition(glm::vec3(0.0f, -5.1f, 0.0f));
	groundEntity->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	groundEntity->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));

	btCollisionShape* groundColShape = new btBoxShape(btVector3(5.0f, 5.0f, 5.0f));
	//Sleepy::CollisionSystem::GetInstance().m_collisionShapes.push_back(groundColShape);
	assert((!groundColShape || groundColShape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -5.1, 0));

	btScalar mass(0.f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundColShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
	
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, groundMotionState, groundColShape, localInertia);

	groundRigidBody = new btRigidBody(cInfo);
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	groundRigidBody->setFriction(btScalar(0.5f));
	groundRigidBody->setRollingFriction(btScalar(0.5f));
	//body->setSpinningFriction(btScalar(0.1f));
	//groundRigidBody->setUserIndex(-1);
	groundRigidBody->setUserPointer(groundEntity);

	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->addRigidBody(groundRigidBody);

	if (Sleepy::Application::s_Playing)
	{
		groundEntity->bActive = true;
		groundEntity->BeginPlay();
	}
}

void CollisionTest::DeleteCube()
{
	delete groundRigidBody->getMotionState();

	btCollisionShape* groundColShape = groundRigidBody->getCollisionShape();

	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody;
	groundRigidBody = nullptr;

	delete groundColShape;

	DeleteEntity(*groundEntity);
	groundEntity = nullptr;
	
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
		//	ballRigidBody->setLinearVelocity(btVector3(dirNorm.x, dirNorm.y, dirNorm.z));
		//ballRigidBody->applyForce(btVector3(dirSpeed.x, dirSpeed.y, dirSpeed.z), btVector3(forcePoint.x, forcePoint.y, forcePoint.z));
		ballRigidBody->applyCentralForce(btVector3(dirSpeed.x, dirSpeed.y, dirSpeed.z));
		ballRigidBody->applyTorque(btVector3(dirSpeed.z, dirSpeed.y, -dirSpeed.x));
		//ballRigidBody->apply
		//ballRigidBody->applyForce(btVector3(dirNorm.x, dirNorm.y, dirNorm.z), btVector3(0.0f, 1.0f, 0.0f));
	}
}

void CollisionTest::StopMoveForward()
{
	dir.z = 0;
}

void CollisionTest::StopMoveBackward()
{
	dir.z = 0;
}

void CollisionTest::StopMoveLeft()
{
	dir.x = 0;
}

void CollisionTest::StopMoveRight()
{
	dir.x = 0;
}
