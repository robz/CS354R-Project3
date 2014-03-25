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
Assignment3::Assignment3()
{
    simulator = NULL;
}

//-------------------------------------------------------------------------------------
Assignment3::~Assignment3(void)
{
}


int startingFace = 0;
bool gameplay = false;
int cPort = 55555;
int sPort = 55554;
char* sip;
char* cip;

//-------------------------------------------------------------------------------------
void Assignment3::createScene(void)
{
	// Initialize CEGUI
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	// Set CEGUI resource groups
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	//Set the CEGUI skin
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	// create window manager and sheet
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *menuSheet = wmgr.createWindow("DefaultWindow", "GUI/Sheet");

	// create buttons
    p1score = wmgr.createWindow("TaharezLook/Button", "UI/Score1");
	p2score = wmgr.createWindow("TaharezLook/Button", "UI/Score2");
	singlebtn = wmgr.createWindow("TaharezLook/Button", "Menu/SingleButton");
	clientbtn = wmgr.createWindow("TaharezLook/Button", "Menu/ClientButton");
	serverbtn = wmgr.createWindow("TaharezLook/Button", "Menu/SeverButton");

	// set buttons sizes and locations
	singlebtn->setText("SINGLE PLAYER");
	singlebtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.05, 0)));
	singlebtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4,0), CEGUI::UDim(0.3, 0)));
	clientbtn->setText("CLIENT");
	clientbtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.05, 0)));
	clientbtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4,0), CEGUI::UDim(0.4, 0)));
	serverbtn->setText("SERVER");
	serverbtn->setSize(CEGUI::UVector2(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.05, 0)));
	serverbtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4,0), CEGUI::UDim(0.55, 0)));
    p1score->setText("PLAYER 1");
    p1score->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	p2score->setText("PLAYER 2");
	p2score->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	p2score->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85,0), CEGUI::UDim(0, 0)));	

    // find the hostname of this machine
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);

	// create text fields for IP addresses and ports
	serverIP = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","SIP"));
	serverIP->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	serverIP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.26,0), CEGUI::UDim(0.47,0)));
	serverIP->setText(hostname);
    cServerPort = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","CSP"));
	cServerPort->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	cServerPort->setPosition(CEGUI::UVector2(CEGUI::UDim(0.415,0), CEGUI::UDim(0.47,0)));
	cServerPort->setText("5555");
	cClientPort = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","CCP"));
	cClientPort->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	cClientPort->setPosition(CEGUI::UVector2(CEGUI::UDim(0.57,0), CEGUI::UDim(0.47,0)));
	cClientPort->setText("5556");
	clientIP = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","CIP"));
	clientIP->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	clientIP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.26,0), CEGUI::UDim(0.62,0)));
	clientIP->setText(hostname);
	sServerPort = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","SSP"));
	sServerPort->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	sServerPort->setPosition(CEGUI::UVector2(CEGUI::UDim(0.415,0), CEGUI::UDim(0.62,0)));
	sServerPort->setText("5556");
	sClientPort = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","SCP"));
	sClientPort->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	sClientPort->setPosition(CEGUI::UVector2(CEGUI::UDim(0.57,0), CEGUI::UDim(0.62,0)));
	sClientPort->setText("5555");

	// add buttons to sheet
	menuSheet->addChildWindow(singlebtn);
	menuSheet->addChildWindow(clientbtn);
	menuSheet->addChildWindow(serverbtn);
	menuSheet->addChildWindow(p1score);
	menuSheet->addChildWindow(p2score);
	menuSheet->addChildWindow(serverIP);
	menuSheet->addChildWindow(cServerPort);
	menuSheet->addChildWindow(cClientPort);
	menuSheet->addChildWindow(clientIP);
	menuSheet->addChildWindow(sServerPort);
	menuSheet->addChildWindow(sClientPort);
    CEGUI::System::getSingleton().setGUISheet(menuSheet);

	// button events
	singlebtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment3::singlePlayer, this));
	clientbtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment3::clientStart, this));
	serverbtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment3::serverStart, this));

    // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
 
  

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(10.0f, 10.0f, 10.0f);
}

void Assignment3::createFrameListener(void) {
	BaseApplication::createFrameListener();
   /* Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);*/
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

	if (gameplay) {
        Surface* paddle = (isClient) ? clientPaddle : serverPaddle;
        
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

        // get a packet from the server, then set the ball's position
        if (isClient) {
            btTransform trans;
            
            // get the state of the ball from the server
            if (netEnt->recMsg(reinterpret_cast<char*>(&trans))) {
                std::cout << "y: " << trans.getOrigin().getY() << std::endl;
                clientBall->getNode().resetToInitialState();
                clientBall->getNode().scale(0.01f, 0.01f, 0.01f);
                clientBall->move(
                    trans.getOrigin().getX(),
                    trans.getOrigin().getY(),
                    trans.getOrigin().getZ()
                    );
            }
    
            // send the state of the paddle to the server
            float pose[7];
            pose[0] = clientPaddle->getNode().getPosition().x;
            pose[1] = clientPaddle->getNode().getPosition().y;
            pose[2] = clientPaddle->getNode().getPosition().z;
            pose[3] = clientPaddle->getNode().getOrientation().w;
            pose[4] = clientPaddle->getNode().getOrientation().x;
            pose[5] = clientPaddle->getNode().getOrientation().y;
            pose[6] = clientPaddle->getNode().getOrientation().z;
            netEnt->sendMsg(reinterpret_cast<char*>(pose), sizeof(pose));
        } else {
            btTransform trans;
            
            // step the server's simulator
            simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);
        
            // send the state of the ball to the client
            clientBall->body->getMotionState()->getWorldTransform(trans);
            netEnt->sendMsg(reinterpret_cast<char*>(&trans), sizeof(btTransform));
        
            // get the state of the paddle from the client
            float pose[7];
            if (netEnt->recMsg(reinterpret_cast<char*>(pose))) {
                clientPaddle->getNode().setPosition(pose[0], pose[1], pose[2]);
                clientPaddle->getNode().setOrientation(pose[3], pose[4], pose[5], pose[6]);
                clientPaddle->updateTransform();
            }
        }

        std::ostringstream stream;
        stream << "score: " << target->wall;
        p1score->setText(stream.str());
    }
    
    return true;
}

bool Assignment3::keyPressed(const OIS::KeyEvent &arg)
{   
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	sys.injectChar(arg.text);

	if (simulator) {
			if (arg.key == OIS::KC_R) {
					simulator->setGravity(simulator->gravity + 20.0);        
			} else if (arg.key == OIS::KC_F) {
					simulator->setGravity(simulator->gravity - 20.0);
			} else if (arg.key == OIS::KC_X) {
					simulator->soundOn = !(simulator->soundOn);
			} else if (arg.key == OIS::KC_C) {
					simulator->soundSystem->playMusic();
			}
	}
	if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
	return true;
}

bool Assignment3::keyReleased(const OIS::KeyEvent &arg)
{
	CEGUI::System::getSingleton().injectKeyUp(arg.key);
	return true;
}

CEGUI::MouseButton Assignment3::convertButton(OIS::MouseButtonID buttonID) {
	switch (buttonID) {
		case OIS::MB_Left:
			return CEGUI::LeftButton;
		case OIS::MB_Right:
			return CEGUI::RightButton;
		case OIS::MB_Middle:
			return CEGUI::MiddleButton;
		default:
			return CEGUI::LeftButton;
	}
}

bool Assignment3::mouseMoved(const OIS::MouseEvent &arg)
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	if (arg.state.Z.rel)
		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}

bool Assignment3::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;
}

bool Assignment3::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}

bool Assignment3::singlePlayer(const CEGUI::EventArgs &e)
{
    gameplay = true;
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	wmgr.destroyWindow(p2score);
	destroyMenu();
	return true;
}

/*  static Server server(port);
    btTransform trans;
    static bool init = false;
    if(!init){
        server.awaitConnections();
        init = true;
    } */

bool Assignment3::clientStart(const CEGUI::EventArgs &e)
{
	isClient = true;
	CEGUI::WindowManager &mgr = CEGUI::WindowManager::getSingleton();
	sPort = atoi(CEGUIStringToString(cServerPort->getText()));
	cPort = atoi(CEGUIStringToString(cClientPort->getText()));
	sip = CEGUIStringToString(serverIP->getText());
	netEnt = new UDPNetEnt(sip, sPort, cPort);
	// Create a scene
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/Frost");
    target = new Target("mytarget", mSceneMgr, simulator, 0, 0, 0, 130, 130, 130, 50);
    target->setPose(startingFace, 0, 0);

    clientBall = new Ball("clientball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 100.0, 0), .9f, .1f, "Examples/RustySteel");
    clientPaddle = new Surface("clientpaddle", mSceneMgr, simulator, 0, 75.0, 20, 10.0, 10.0, 2.5, 0.25, 0.1, "Examples/BumpyMetal");
    
    //Setup player camera
    (&(clientPaddle->getNode()))->createChildSceneNode("camNode");
    mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);
	destroyMenu();
	gameplay = true;
	
    return true;
}

bool Assignment3::serverStart(const CEGUI::EventArgs &e)
{
	isClient = false;
	CEGUI::WindowManager &mgr = CEGUI::WindowManager::getSingleton();
	sPort = atoi(CEGUIStringToString(sServerPort->getText()));
	cPort = atoi(CEGUIStringToString(sClientPort->getText()));
	cip = CEGUIStringToString(clientIP->getText());
	netEnt = new UDPNetEnt(cip, sPort, cPort);
	simulator = new Simulator();
  
    // Create a scene
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/Frost");
    target = new Target("mytarget", mSceneMgr, simulator, 0, 0, 0, 130, 130, 130, 50);
    target->setPose(startingFace, 0, 0);
    
    serverBall = new Ball("serverball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 70.0, 0), .9f, .1f, "Examples/RustySteel");
    serverPaddle = new Surface("serverpaddle", mSceneMgr, simulator, 0, 75.0, 20, 10.0, 10.0, 2.5, 0.25, 0.1, "Examples/BumpyMetal");
    
    clientBall = new Ball("clientball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 100.0, 0), .9f, .1f, "Examples/RustySteel");
    clientPaddle = new Surface("clientpaddle", mSceneMgr, simulator, 0, 75.0, 40, 10.0, 10.0, 2.5, 0.25, 0.1, "Examples/BumpyMetal");
    
    //Setup player camera
    (&(serverPaddle->getNode()))->createChildSceneNode("camNode");
    mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);
	
	box->addToSimulator();
	target->addToSimulator();
	target->setKinematic();
	
    clientBall->addToSimulator();
	clientPaddle->addToSimulator();
	clientPaddle->setKinematic();
    
    serverBall->addToSimulator();
	serverPaddle->addToSimulator();
	serverPaddle->setKinematic();
    
    destroyMenu();
	gameplay = true;
	return true;
}

void Assignment3::destroyMenu()
{
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	wmgr.destroyWindow(singlebtn);
	wmgr.destroyWindow(clientbtn);
	wmgr.destroyWindow(serverbtn);
	wmgr.destroyWindow(serverIP);
	wmgr.destroyWindow(cServerPort);
	wmgr.destroyWindow(cClientPort);
	wmgr.destroyWindow(clientIP);
	wmgr.destroyWindow(sServerPort);
	wmgr.destroyWindow(sClientPort);
	CEGUI::MouseCursor::getSingleton().hide();
}

char * Assignment3::CEGUIStringToString(CEGUI::String cestr)
{
	char * newstr = (char *) malloc((cestr.size()+1) * sizeof(char));
	memset(newstr,0xFF,cestr.size()+1);
	for(int i = 0; i < cestr.size(); i++) {
		newstr[i] &= cestr[i];
	}
	newstr[cestr.size()] = 0;
	return newstr;
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
