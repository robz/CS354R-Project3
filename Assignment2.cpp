/*
-----------------------------------------------------------------------------
Filename:    Assignment2.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "Assignment2.h"
#include "PlayingField.h"

//-------------------------------------------------------------------------------------
Assignment2::Assignment2(void)
{
    groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));

    groundRigidBodyCI.m_restitution = 0.9f;
    groundRigidBodyCI.m_friction = 0.1f;
    groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);


    fallShape = new btSphereShape(1);
    fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,100,0)));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    fallRigidBodyCI.m_restitution = 0.9f;
    fallRigidBodyCI.m_friction = 0.1f;
    fallRigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody);
}
//-------------------------------------------------------------------------------------
Assignment2::~Assignment2(void)
{
    dynamicsWorld->removeRigidBody(fallRigidBody);
    delete fallRigidBody->getMotionState();
    delete fallRigidBody;

    dynamicsWorld->removeRigidBody(groundRigidBody);
    delete groundRigidBody->getMotionState();
    delete groundRigidBody;

    delete fallShape;
    delete groundShape;

    delete simulator;
}

//-------------------------------------------------------------------------------------
void Assignment2::createScene(void)
{
    simulator = new Simulator();

    // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
 
    // Create a scene
    PlayingField* bCourt = new PlayingField(mSceneMgr);
    ball = new Ball(mSceneMgr);
    bCourt->addChild(ball->getNode());
    ball->setPlayingField(bCourt);
     
    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(100.0f, 100.0f, 100.0f);
}

bool Assignment2::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;
    
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);
    if (!mTrayMgr->isDialogVisible()) {
        // if dialog isn't up, then update the scene
        mCameraMan->frameRenderingQueued(evt);
    
        // Step the simulation
        dynamicsWorld->stepSimulation(1/60.f,10);
        
        // Set the ball's new position    
        btTransform trans;
        fallRigidBody->getMotionState()->getWorldTransform(trans);
        ball->getNode()->setPosition(
            trans.getOrigin().getX(),
            trans.getOrigin().getY(),
            trans.getOrigin().getZ()
            );
            
    }
    return true;
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Assignment2 app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
