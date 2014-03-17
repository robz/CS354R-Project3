/*
-----------------------------------------------------------------------------
Filename:    Assignment3.cpp
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
#include "Assignment3.h"

//-------------------------------------------------------------------------------------
Assignment3::Assignment3(void)
{
}

//-------------------------------------------------------------------------------------
Assignment3::~Assignment3(void)
{
    delete simulator;
}


int startingFace = 0;

//-------------------------------------------------------------------------------------
void Assignment3::createScene(void)
{
    simulator = new Simulator();

    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");

    // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
 
    // Create a scene
    ball = new Ball("myball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 20.0, 0), .9f, .1f, "Examples/RustySteel");
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/Frost");
    paddle = new Surface("mypaddle", mSceneMgr, simulator, 0, 75.0, 20, 10.0, 10.0, 2.5, 0.25, 0.1, "Examples/BumpyMetal");
    target = new Target("mytarget", mSceneMgr, simulator, 0, 0, 0, 130, 130, 130, 50, ball->body);

    target->setPose(startingFace, 0, 0);
 
    ball->addToSimulator();
    box->addToSimulator();
    paddle->addToSimulator();
    paddle->setKinematic();
    target->addToSimulator();
    target->setKinematic();

    //Setup player camera
    (&(paddle->getNode()))->createChildSceneNode("camNode");
    mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(10.0f, 10.0f, 10.0f);
   
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    score = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    score->setText("SCORE!");
    score->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    sheet->addChildWindow(score);
    CEGUI::System::getSingleton().setGUISheet(sheet);
}

float PADDLE_X_SPEED = 60.0f,
      PADDLE_Y_SPEED = 60.0f,
      PADDLE_Z_SPEED = 60.0f,
      PADDLE_ROT_SPEED = 30.0f;

bool Assignment3::frameRenderingQueued(const Ogre::FrameEvent& evt) {

    static Ogre::Real z_time = 0.0;

    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;
    
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
    //mTrayMgr->frameRenderingQueued(evt);
    if (!mTrayMgr->isDialogVisible()) {
        // if dialog isn't up, then update the scene
        //mCameraMan->frameRenderingQueued(evt);
    
        // Step the simulation
        if(mKeyboard->isKeyDown(OIS::KC_Z) && z_time == 0.0){
            paddle->rotate(180, 0.0, 0.0);
            z_time += evt.timeSinceLastFrame;
        }
        if(z_time > 0.0 && z_time < 1.0)
            z_time += evt.timeSinceLastFrame;
        else
            z_time = 0.0;
        if(mKeyboard->isKeyDown(OIS::KC_LSHIFT)){
            if(mKeyboard->isKeyDown(OIS::KC_W)){
                paddle->move(0.0, PADDLE_Y_SPEED * evt.timeSinceLastFrame, 0.0);
            }
            if (mKeyboard->isKeyDown(OIS::KC_S)){
                paddle->move(0.0, -PADDLE_Y_SPEED * evt.timeSinceLastFrame, 0.0);
            }
        }
        else{
            if(mKeyboard->isKeyDown(OIS::KC_W)){
                paddle->move(0.0, 0.0, -PADDLE_Z_SPEED * evt.timeSinceLastFrame);
            }
            if (mKeyboard->isKeyDown(OIS::KC_S)){
                paddle->move(0.0, 0.0, PADDLE_Z_SPEED * evt.timeSinceLastFrame);
            }
        }
        if (mKeyboard->isKeyDown(OIS::KC_A)){
            paddle->move(-PADDLE_X_SPEED * evt.timeSinceLastFrame, 0.0, 0.0);
        }
        if (mKeyboard->isKeyDown(OIS::KC_D)){
            paddle->move(PADDLE_X_SPEED * evt.timeSinceLastFrame, 0.0, 0.0);
        }
        if (mKeyboard->isKeyDown(OIS::KC_Q)){
            paddle->rotate(0.0, 0.0, PADDLE_ROT_SPEED * evt.timeSinceLastFrame);
        }
        if (mKeyboard->isKeyDown(OIS::KC_E)){
            paddle->rotate(0.0, 0.0, -PADDLE_ROT_SPEED * evt.timeSinceLastFrame);
        }
        
        Ogre::Real xMove = mMouse->getMouseState().X.rel;
        Ogre::Real yMove = mMouse->getMouseState().Y.rel;
        paddle->rotate(-xMove*0.1, -yMove*0.1, 0.0, Ogre::Node::TS_WORLD);
        paddle->updateTransform();
        simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);
        
        std::ostringstream stream;
        stream << "score: " << target->wall;
        score->setText(stream.str());
        
    }
    return true;
}

bool Assignment3::keyPressed(const OIS::KeyEvent &arg)
{   
    if (arg.key == OIS::KC_R) {
        simulator->setGravity(simulator->gravity + 20.0);        
    } else if (arg.key == OIS::KC_F) {
        simulator->setGravity(simulator->gravity - 20.0);
    } else if (arg.key == OIS::KC_X) {
        simulator->soundOn = !(simulator->soundOn);
    } else if (arg.key == OIS::KC_C) {
        simulator->soundSystem->playMusic();
    }

    return BaseApplication::keyPressed(arg);
}

bool Assignment3::keyReleased(const OIS::KeyEvent &arg)
{
    return BaseApplication::keyReleased(arg);
}


bool Assignment3::mouseMoved(const OIS::MouseEvent &arg)
{
    return BaseApplication::mouseMoved(arg);
}

bool Assignment3::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return BaseApplication::mousePressed(arg, id);
}

bool Assignment3::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return BaseApplication::mouseReleased(arg, id);
}

bool Assignment3::quit(const CEGUI::EventArgs &e)
{
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
        if (argc == 2) {
            startingFace = atoi(argv[1]);
        }

        // Create application object
        Assignment3 app;

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
