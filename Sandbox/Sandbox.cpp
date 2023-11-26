// Sandbox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Application.h>

#include "SandboxScene.h"

int main()
{


    //btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    //btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    //btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    //btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
    //    overlappingPairCache, solver, collisionConfiguration);

    //dynamicsWorld->setGravity(btVector3(0, -10, 0));

    //for (int i = 0; i < 100; i++)
    //{
    //    dynamicsWorld->stepSimulation(1.f / 60.f, 10);
    //
    //    // print positions of all objects
    //    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    //    {
    //        btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[j];
    //        btRigidBody * body = btRigidBody::upcast(obj);
    //        btTransform trans;
    //        if (body && body->getMotionState())
    //        {
    //            body->getMotionState()->getWorldTransform(trans);
    //        }
    //        else
    //        {
    //            trans = obj->getWorldTransform();
    //        }
    //        printf(" world pos object %d = %f ,%f ,%f\n", j, float(trans.getOrigin().getX()), float(
    //        trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    //    }
    //}
    Sleepy::Application app;
    SandboxScene* scene = new SandboxScene();
    app.SetScene(scene);
    app.Run();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
