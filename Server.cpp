#include <unistd.h>
#include "Server.h"

void onlineTutorial(int argc, char* argv[]) {
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-10,0));


    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

    btCollisionShape* fallShape = new btSphereShape(1);


    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);


    btDefaultMotionState* fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody);


    Server server(argv[1], atoi(argv[2]));
    btTransform trans;
    
    for (int i = 0; i < 300; i++) {
            dynamicsWorld->stepSimulation(1/60.f,10);

            btTransform trans;
            fallRigidBody->getMotionState()->getWorldTransform(trans);
            std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
        
            server.sendMsg(reinterpret_cast<char*>(&trans), sizeof(btTransform));
            usleep(1000000/60);
    }

    dynamicsWorld->removeRigidBody(fallRigidBody);
    delete fallRigidBody->getMotionState();
    delete fallRigidBody;

    dynamicsWorld->removeRigidBody(groundRigidBody);
    delete groundRigidBody->getMotionState();
    delete groundRigidBody;


    delete fallShape;

    delete groundShape;


    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

int main(int argc, char* argv[]) {
    while (true) {
        onlineTutorial(argc, argv);
    }
    /*
    Simulator* simulator = new Simulator();
    
    Ball* ball = new Ball("myball", NULL, simulator, 1.0, 1.0, Ogre::Vector3(0, 20.0, 0), .9f, .1f, "Examples/RustySteel");
    
    ball->addToSimulator();
    
    while (true) {
        simulator->stepSimulation(1/60.f, 10);
        
        // send state of ball
        ball->body->getMotionState()->getWorldTransform(trans);
        std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
        server.sendMsg(reinterpret_cast<char*>(&trans), sizeof(btTransform));
        
    }
    */
}
