#include "CollisionSystem.h"
#include <iostream>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "Entity.h"
namespace Sleepy
{

	void CollisionSystem::InitPhysics()
	{
		GetInstance().InitPhysicsImpl();
	}

	void CollisionSystem::EndSimulation()
	{
		GetInstance().EndSimulationImpl();
	}

	void CollisionSystem::DeleteAll()
	{
		GetInstance().DeleteAllImpl();
	}

	void CollisionSystem::Update(float deltaTime)
	{
		GetInstance().UpdateImpl(deltaTime);
	}

	void CollisionSystem::UpdateImpl(float deltaTime)
	{
		dynamicsWorld->stepSimulation(deltaTime);

		// print positions of all objects
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btGhostObject* ghost = btGhostObject::upcast(obj);

			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
				dynamicsWorld->updateAabbs();
			}
			else if(ghost)
			{
				btCollisionObjectArray arr = ghost->getOverlappingPairs();
				
				for(int i=0; i< arr.size(); i++)
				{
					Entity* ent = (Entity*)(arr[0]->getUserPointer());
					if(ent)
						ent->OnOverlap();
				}
				trans = obj->getWorldTransform();
			}
			//printf(" world pos object %d = %f ,%f ,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ
			printf("Collision objects: %d \n", dynamicsWorld->getNumCollisionObjects());
		}
	}

	void CollisionSystem::InitPhysicsImpl()
	{
		/// collision configuration contains default setup for memory , collision setup . Advanced users can create their own configuration .
		collisionConfiguration = new btDefaultCollisionConfiguration();

		dispatcher = new btCollisionDispatcher(collisionConfiguration);

		overlappingPairCache = new btDbvtBroadphase();

		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
			overlappingPairCache, solver, collisionConfiguration);

		dynamicsWorld->setGravity(btVector3(0, -10, 0));

		//specifically for boids, was removed so should probably remove again?
		m_collisionShapes.push_back(new btSphereShape(btScalar(0.35f)));

		
		///// Create Dynamic Objects
		//btTransform startTransform;
		//startTransform.setIdentity();

		//btScalar mass(1.f);

		////rigidbody is dynamic if and only if mass is non zero, otherwise static
		//bool isDynamic = (mass != 0.f);

		//btVector3 localInertia(0, 0, 0);
		//if (isDynamic)
		//	colShape->calculateLocalInertia(mass, localInertia);

		//startTransform.setOrigin(btVector3(2, 10, 0));

		////using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		//btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		//btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		//btRigidBody* body = new btRigidBody(rbInfo);
		//body->applyCentralForce(btVector3(0.0f, 0.0f, 0.5f));
		//dynamicsWorld->addRigidBody(body);


		///// Create Dynamic Objects
		//btTransform startTransform2;

		//startTransform2.setIdentity();

		//btScalar mass2(0.f);

		////rigidbody is dynamic if and only if mass is non zero, otherwise static
		//isDynamic = (mass2 != 0.f);

		//btVector3 localInertia2(0, 0, 0);
		//if (isDynamic)
		//	colShape2->calculateLocalInertia(mass2, localInertia2);

		//startTransform2.setOrigin(btVector3(2, 0, 0));

		////using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		//myMotionState = new btDefaultMotionState(startTransform2);
		//btRigidBody::btRigidBodyConstructionInfo rbInfo2(mass2, myMotionState, colShape2, localInertia2);
		//btRigidBody* body2 = new btRigidBody(rbInfo2);

		//dynamicsWorld->addRigidBody(body2);

	}


	void CollisionSystem::DeleteAllImpl()
	{
		// remove the rigidbodies from the dynamics world and delete them
		for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		// delete collision shapes
		for (int j = 0; j < m_collisionShapes.size(); j++)
		{
			btCollisionShape* shape = m_collisionShapes[j];
			m_collisionShapes[j] = 0;
			delete shape;
		}

		// delete dynamics world
		delete dynamicsWorld;

		// delete solver
		delete solver;

		// delete broadphase
		delete overlappingPairCache;

		// delete dispatcher
		delete dispatcher;

		delete collisionConfiguration;

		// next line is optional : it will be cleared by the destructor when the array goes out of scope

		m_collisionShapes.clear();

	}


	btCollisionShape* CollisionSystem::GetSphereCollisionShape()
	{
		return m_collisionShapes[0];
	}


	void CollisionSystem::EndSimulationImpl()
	{
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->clearForces();
				btVector3 zeroVector(0, 0, 0);
				body->setLinearVelocity(zeroVector);
				body->setAngularVelocity(zeroVector);
			}

			//printf(" world pos object %d = %f ,%f ,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}
	}
}