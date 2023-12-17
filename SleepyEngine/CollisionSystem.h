#pragma once

#include "btBulletDynamicsCommon.h"
namespace Sleepy
{
	class CollisionSystem;
}
static Sleepy::CollisionSystem* s_CollisionSystem;
namespace Sleepy
{
	class CollisionSystem
	{
	public:
		static CollisionSystem& GetInstance() {
			if (!s_CollisionSystem)
				s_CollisionSystem = new CollisionSystem();
			return *s_CollisionSystem;
		}

		void Update(float deltaTime);
		
		void InitPhysics();
		static void EndSimulation();
		void DeleteAll();

		btCollisionShape* GetSphereCollisionShape();

		btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
		btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
		btCollisionDispatcher* dispatcher = nullptr;
		btBroadphaseInterface* overlappingPairCache = nullptr;
		btSequentialImpulseConstraintSolver* solver = nullptr;
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
		friend class Application;

	private:

		CollisionSystem() {};

		void UpdateImpl(float deltaTime);
		void EndSimulationImpl();
		void InitPhysicsImpl();
		void DeleteAllImpl();
	};

}
