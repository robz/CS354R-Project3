#include <iostream>
#include "Simulator.h"
#include "ContactSensorCallback.h"

Simulator::Simulator(){
	// Collision configuration contains default setup for memory, collision setup.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	// Use the default collision dispatcher
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase. Can also try btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	//the default constraint solver. For parallel processing you can use different solver.
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(-9.8f, 9.8f, -9.8f));

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
}

Simulator::~Simulator() {

}

void Simulator::addObject (GameObject* o){
	objList.push_back(o->callback);
	
    //use default collision group/mask values (dynamic/kinematic/static)
	dynamicsWorld->addRigidBody(o->getBody());
}

bool Simulator::removeObject(GameObject* o) {

}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep){
	//do we need to update positions in simulator for dynamic objects?
	dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);

    for (int i = 0; i < objList.size(); i++) {
        for (int j = 0; j < objList.size(); j++) {
            if (i == j) { continue; }
            
            objList[i]->ctxt.hit = false;

            dynamicsWorld->contactPairTest(
                &(objList[i]->body), 
                &(objList[j]->body), 
                *objList[i]
                ); 
            
            if (objList[i]->ctxt.hit) {
                std::cout << "bounce" << std::endl;
            } 
        }
    }
}
