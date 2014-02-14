#include "Simulator.h"

Simulator::Simulator() {
    //collision conﬁguration contains default setup for memory, collision setup.
    collisionConﬁguration = new btDefaultCollisionConﬁguration();
    //use the default collision dispatcher. For parallel processing you can use a diffent dispatcher
    dispatcher = new btCollisionDispatcher(collisionConﬁguration);
    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();
    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConﬁguration);
    dynamicsWorld->setGravity(btVector3(0,-0.098, 0));
    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
}
    
void Simulator::addObject (GameObject* o) {
    objList.push_back(o);
    // use default collision group/mask values (dynamic/kinematic/static)
    dynamicsWorld->addRigidBody(o->getBody()); 
}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real ﬁxedTimestep) {
    // do we need to update positions in simulator for dynamic objects?
    dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, ﬁxedTimestep);
}
