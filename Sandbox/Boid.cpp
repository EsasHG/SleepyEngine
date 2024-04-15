#include "Boid.h"
#include <TransformSystem.h>
#include <CollisionSystem.h>
#include <glm/gtx/norm.hpp>

Boid::~Boid()
{
	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->removeCollisionObject(rigidBody);
	delete rigidBody;
	delete myMotionState;
}

void Boid::BeginPlay()
{
	Sleepy::Entity::BeginPlay();


	//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
	//Sleepy::CollisionSystem::GetInstance().m_collisionShapes.push_back(colShape);

	btCollisionShape* colShape = Sleepy::CollisionSystem::GetInstance().GetSphereCollisionShape();


	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(1.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(2, 5, 0));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	rigidBody = new btRigidBody(rbInfo);
	//rigidBody->applyCentralForce(btVector3(0.0f, 0.0f, 0.5f));
	Sleepy::CollisionSystem::GetInstance().dynamicsWorld->addRigidBody(rigidBody);

	rigidBody->setGravity(btVector3(0.0f, -0.0f, 0.0f));

}

void Boid::Update(double deltaTime)
{
	Sleepy::Entity::Update(deltaTime);
	
	glm::vec3 pos = GetPosition();

	if (pos.x < -boidInfo.margin)
		velocity.x = velocity.x + boidInfo.turnFactor;
	if (pos.x > boidInfo.margin)
		velocity.x = velocity.x - boidInfo.turnFactor;
	if (pos.y > boidInfo.margin)
		velocity.y = velocity.y - boidInfo.turnFactor;
	if (pos.y < -boidInfo.margin)
		velocity.y = velocity.y + boidInfo.turnFactor;
	if (pos.z > boidInfo.margin)
		velocity.z = velocity.z - boidInfo.turnFactor;
	if (pos.z < -boidInfo.margin)
		velocity.z = velocity.z + boidInfo.turnFactor;

	float speed2 = glm::abs(glm::dot(velocity, velocity));
	if (speed2 == 0) return;
	if (speed2 > boidInfo.maxSpeed * boidInfo.maxSpeed)
		velocity = glm::normalize(velocity) * boidInfo.maxSpeed;
	if (speed2 < boidInfo.minSpeed * boidInfo.minSpeed)
		velocity = glm::normalize(velocity) * boidInfo.minSpeed;
	//SetPosition(GetPosition() + (float(deltaTime) * velocity));


	if (glm::length2(velocity) > 0)
	{
		velocity = glm::normalize(velocity) * 5.0f;
		//velocity = glm::normalize(velocity) * 50.0f;
		rigidBody->applyCentralForce(btVector3(velocity.x, velocity.y, velocity.z));
	}
	btTransform trans;

	rigidBody->getMotionState()->getWorldTransform(trans);

	btVector3 btPos = trans.getOrigin();
	SetPosition(glm::vec3(btPos.x(), btPos.y(), btPos.z()));

	//SetPosition(pos + (float(deltaTime) * velocity));
	velocity = glm::vec3(0.0f);



}

void Boid::CheckOthers(std::vector<Boid*>& boids)
{
	glm::vec3 pos = GetPosition();

	glm::vec3 avoidVec = glm::vec3(0);
	glm::vec3 avgDir = glm::vec3(0);
	glm::vec3 avgPos = glm::vec3(0);
	float neighbouringBoids = 0.0f;

	for (auto boid : boids)
	{
		if (boid == this)
			continue;

		glm::vec3 otherPos = boid->GetPosition();
		glm::vec3 dir = (pos - otherPos);
		float distance2 = glm::abs(glm::dot(dir, dir));

		if (distance2 < boidInfo.minDistance * boidInfo.minDistance)
		{
			avoidVec += dir;
		}
		else if (distance2 < boidInfo.viewRange * boidInfo.viewRange)
		{
			avgDir += boid->velocity;
			avgPos += boid->GetPosition();
			neighbouringBoids++;
		}
	}
	velocity += avoidVec * boidInfo.avoidFactor;
	if (neighbouringBoids > 0)
	{
		avgDir = avgDir / neighbouringBoids;
		velocity += (avgDir - velocity) * boidInfo.matchingFactor;
		avgPos += avgPos / neighbouringBoids;
		velocity += (avgPos - pos) * boidInfo.centeringFactor;
	}
	if (inGroup1)
		velocity = (1 - boidInfo.biasVal) * velocity + (boidInfo.biasVal * 1.0f);
	else if (inGroup2)
		velocity = (1 - boidInfo.biasVal) * velocity + (boidInfo.biasVal * -1.0f);


}
