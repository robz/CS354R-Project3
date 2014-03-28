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
bool isSinglePlayer = false;
int sPort = 49152;
char* sip;

//-------------------------------------------------------------------------------------
void Assignment3::createScene(void)
{
	CEGUI::Event::Subscriber* spSub = new CEGUI::Event::Subscriber(&Assignment3::singlePlayer, this);
	CEGUI::Event::Subscriber* clientSub = new CEGUI::Event::Subscriber(&Assignment3::clientStart, this);
	CEGUI::Event::Subscriber* serverSub = new CEGUI::Event::Subscriber(&Assignment3::serverStart, this);
	//c = &Assignment3::clientStart();
	gui = new GUI(spSub, clientSub, serverSub);
	//CEGUI::Event::Subscriber* s = new CEGUI::Event::Subscriber(&Assignment3::singlePlayer, this);
	//gui->spBTN->subscribeEvent(CEGUI::PushButton::EventClicked, s);
//	gui->clientBTN->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment3::clientStart, this));
//	gui->serverBTN->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment3::serverStart, this));

/*
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
	serverbtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4,0), CEGUI::UDim(0.5, 0)));
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
	serverIP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.32,0), CEGUI::UDim(0.65,0)));
	serverIP->setText(hostname);
	
	serverPort = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox","SPORT"));
	serverPort->setSize(CEGUI::UVector2(CEGUI::UDim(0.15,0), CEGUI::UDim(0.05,0)));
	serverPort->setPosition(CEGUI::UVector2(CEGUI::UDim(0.52,0), CEGUI::UDim(0.65,0)));
	serverPort->setText("55554");

	// add buttons to sheet
	menuSheet->addChildWindow(singlebtn);
	menuSheet->addChildWindow(clientbtn);
	menuSheet->addChildWindow(serverbtn);
	menuSheet->addChildWindow(p1score);
	menuSheet->addChildWindow(p2score);
	menuSheet->addChildWindow(serverIP);
	menuSheet->addChildWindow(serverPort);
    CEGUI::System::getSingleton().setGUISheet(menuSheet);

	// button events
	singlebtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment3::singlePlayer, this));
	clientbtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment3::clientStart, this));
	serverbtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment3::serverStart, this));
*/
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
      PADDLE_ROT_SPEED = 30.0f,
      HELI_SPEED = 60.0f;

bool Assignment3::frameRenderingQueued(const Ogre::FrameEvent& evt) {

    static Ogre::Real z_time = 0.0;
    static Ogre::Real heli_time = 0.0;
    static bool heli_init = false;
    static int rotate_time = 0;

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
        if(heli_time < 10.0){
            heli_time+= evt.timeSinceLastFrame;
        }
        else{
            if(!heli_init){
                heli = new Heli("dachoppa", mSceneMgr, simulator, 2.0, 1.0, Ogre::Vector3(0.0, -10.0, 100.0), 0.9, 0.1, "");
                heli_init = true;
            }
            if(heli_time < 12.0){
                heli->move(0.0,HELI_SPEED * evt.timeSinceLastFrame,0.0);
                heli_time += evt.timeSinceLastFrame;
            }
            else if(heli_time < 100.0);
                if(rotate_time < 5){
                    //heli->rotate(0.0,-30.0/5.0,0.0);
                    rotate_time++;
                }
                heli->move(0.0,0.0,-HELI_SPEED * evt.timeSinceLastFrame);
                heli_time += evt.timeSinceLastFrame;
        }
        
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
            ServerToClient servData;
            
            // get data from the server
            if (client->recMsg(reinterpret_cast<char*>(&servData))) {
                // update our (client's) ball from server
                btTransform& cbTrans = servData.getClientBall();
                clientBall->getNode().resetToInitialState();
                clientBall->getNode().scale(0.01f, 0.01f, 0.01f);
                clientBall->move(
                    cbTrans.getOrigin().getX(),
                    cbTrans.getOrigin().getY(),
                    cbTrans.getOrigin().getZ()
                    );

                // update our (client's) ball from server
                btTransform& sbTrans = servData.getServerBall();
                serverBall->getNode().resetToInitialState();
                serverBall->getNode().scale(0.01f, 0.01f, 0.01f);
                serverBall->move(
                    sbTrans.getOrigin().getX(),
                    sbTrans.getOrigin().getY(),
                    sbTrans.getOrigin().getZ()
                    );

                // update server player's paddle
                float* padPose = servData.getServerPaddle();
                serverPaddle->getNode().setPosition(padPose[0], padPose[1], padPose[2]);
                serverPaddle->getNode().setOrientation(padPose[3], padPose[4], padPose[5], padPose[6]);
                serverPaddle->updateTransform();
                
                // update target from server
                float* targPose = servData.getTarget();
                target->getNode().setPosition(targPose[0], targPose[1], targPose[2]);
                target->getNode().setOrientation(targPose[3], targPose[4], targPose[5], targPose[6]);
                target->updateTransform();

                //play sounds (if any)
                if(simulator->soundOn){
                    int sound = servData.getSound();
                    if(sound == BALLWALL)
                        simulator->soundSystem->playWallHit();
                    else if(sound == BALLTARGET)
                        simulator->soundSystem->playTargetHit();
                    else if(sound == CLIENTBALLPADDLE)
                        simulator->soundSystem->playP2Hit();
					else if (sound == SERVERBALLPADDLE)
						simulator->soundSystem->playRaquetHit();
                }

                //update score
                int* score = servData.getScore();
                gui->setMultiplayerScores(score[0], score[1]);
			}
    
            // send the state of our paddle to the server
            float pose[7];
            pose[0] = clientPaddle->getNode().getPosition().x;
            pose[1] = clientPaddle->getNode().getPosition().y;
            pose[2] = clientPaddle->getNode().getPosition().z;
            pose[3] = clientPaddle->getNode().getOrientation().w;
            pose[4] = clientPaddle->getNode().getOrientation().x;
            pose[5] = clientPaddle->getNode().getOrientation().y;
            pose[6] = clientPaddle->getNode().getOrientation().z;
            client->sendMsg(reinterpret_cast<char*>(pose), sizeof(pose));
        } else {
            if(!isSinglePlayer){
                //btTransform trans; 
                server->awaitConnections();
                // step the server's simulator
                simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);
                // send the state of the ball to the client
                //ball->body->getMotionState()->getWorldTransform(trans);
                //server->sendMsg(reinterpret_cast<char*>(&trans), sizeof(btTransform));
                // send the state of the target to the client
                ServerToClient* data = initServerToClient();
                server->sendMsg(reinterpret_cast<char*>(data), sizeof(ServerToClient));
                delete data;
            
                // get the state of the paddle from the client
                float pose[7];
                if (server->recMsg(reinterpret_cast<char*>(pose))) {
                    clientPaddle->getNode().setPosition(pose[0], pose[1], pose[2]);
                    clientPaddle->getNode().setOrientation(pose[3], pose[4], pose[5], pose[6]);
                    clientPaddle->updateTransform();
                }
            }
            else
                simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);
        }

		if(!isClient){
    		if (isSinglePlayer) {
				gui->setSinglePlayerScore(serverBall->getScore());
			} else {
				gui->setMultiplayerScores(serverBall->getScore(), clientBall->getScore());
			}
        }
	}
    
    return true;
}

ServerToClient* Assignment3::initServerToClient(){
    ServerToClient* data = new ServerToClient();
    
    //server ball information
    btTransform serverBallTrans;
    serverBall->body->getMotionState()->getWorldTransform(serverBallTrans);

    //client ball information
    btTransform clientBallTrans;
    clientBall->body->getMotionState()->getWorldTransform(clientBallTrans);

    //target information 
    float targPose[7];
    targPose[0] = target->getNode().getPosition().x;
    targPose[1] = target->getNode().getPosition().y;
    targPose[2] = target->getNode().getPosition().z;
    targPose[3] = target->getNode().getOrientation().w;
    targPose[4] = target->getNode().getOrientation().x;
    targPose[5] = target->getNode().getOrientation().y;
    targPose[6] = target->getNode().getOrientation().z;

    //server paddle information
    float padPose[7];
    padPose[0] = serverPaddle->getNode().getPosition().x;
    padPose[1] = serverPaddle->getNode().getPosition().y;
    padPose[2] = serverPaddle->getNode().getPosition().z;
    padPose[3] = serverPaddle->getNode().getOrientation().w;
    padPose[4] = serverPaddle->getNode().getOrientation().x;
    padPose[5] = serverPaddle->getNode().getOrientation().y;
    padPose[6] = serverPaddle->getNode().getOrientation().z;

    //sound information
    int sound = simulator->soundPlayed;

    //score information
    int score[2];
    score[0] = serverBall->getScore();
    score[1] = clientBall->getScore();

    data->setSound(sound);
    data->setScore(score);
    data->setServerBall(serverBallTrans);
    data->setClientBall(clientBallTrans);
    data->setTarget(targPose);
    data->setPaddle(padPose);

    return data;
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
    isClient = false;
    isSinglePlayer = true;
	//CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//wmgr.destroyWindow(p2score);

    simulator = new Simulator();
  
    // Create a scene
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/BeachStones");
    target = new Target("mytarget", mSceneMgr, simulator, 0, 0, 0, 130, 130, 130, 50);
    target->setPose(startingFace, 0, 0);
    
    serverBall = new Ball("serverball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 70.0, 0), .9f, .1f, "Game/P1ball");
    serverPaddle = new Surface("serverpaddle", mSceneMgr, simulator, 0, 75.0, 20, 10.0, 10.0, 2.5, 0.25, 0.1, "Game/P1paddle");
    
    //Setup player camera
    (&(serverPaddle->getNode()))->createChildSceneNode("camNode");
    mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);
    
    box->addToSimulator();
    target->addToSimulator();
    target->setKinematic();
    
    serverBall->addToSimulator();
    serverPaddle->addToSimulator();
    serverPaddle->setKinematic();
    
	gui->destroyMenu(true);
    gameplay = true;
    return true;
}

bool Assignment3::clientStart(const CEGUI::EventArgs &e)
{
	isClient = true;
	sPort = gui->getPort();
	sip = gui->getIP();
    client = new Client(sip, sPort);
    
    simulator = new Simulator();
	
    // Create a scene
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/BeachStones");
    target = new Target("mytarget", mSceneMgr, simulator, 0, 0, 0, 130, 130, 130, 50);
    target->setPose(startingFace, 0, 0);
    
    serverBall = new Ball("serverball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 70.0, 0), .9f, .1f, "Game/P1ball");
    serverPaddle = new Surface("serverpaddle", mSceneMgr, simulator, 0, 75.0, 20, 10.0, 10.0, 2.5, 0.25, 0.1, "Game/P1paddle");

    clientBall = new Ball("clientball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 100.0, 0), .9f, .1f, "Game/P2ball");
    clientPaddle = new Surface("clientpaddle", mSceneMgr, simulator, 0, 75.0, 20, 10.0, 10.0, 2.5, 0.25, 0.1, "Game/P2paddle");
    
    //Setup player camera
    (&(clientPaddle->getNode()))->createChildSceneNode("camNode");
    mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);
	gui->destroyMenu(false);
	gameplay = true;
	
    return true;
}

bool Assignment3::serverStart(const CEGUI::EventArgs &e)
{
	isClient = false;
	
	//CEGUI::WindowManager &mgr = CEGUI::WindowManager::getSingleton();
	//sPort = atoi(CEGUIStringToString(serverPort->getText()));
	sPort = gui->getPort();
	//netEnt = new UDPNetEnt(cip, sPort, cPort);
    server = new Server(sPort);
	
    simulator = new Simulator();
  
    // Create a scene
    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/BeachStones");
    target = new Target("mytarget", mSceneMgr, simulator, 0, 0, 0, 130, 130, 130, 50);
    target->setPose(startingFace, 0, 0);
    
    serverBall = new Ball("serverball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 70.0, 0), .9f, .1f, "Game/P1ball");
    serverPaddle = new Surface("serverpaddle", mSceneMgr, simulator, 0, 75.0, 20, 10.0, 10.0, 2.5, 0.25, 0.1, "Game/P1paddle");
    
    clientBall = new Ball("clientball", mSceneMgr, simulator, 1.0, 1.0, Ogre::Vector3(0, 100.0, 0), .9f, .1f, "Game/P2ball");
    clientPaddle = new Surface("clientpaddle", mSceneMgr, simulator, 0, 75.0, 40, 10.0, 10.0, 2.5, 0.25, 0.1, "Game/P2paddle");
    
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
    
	gui->destroyMenu(false);
	gameplay = true;
    printf("Server starting up...\n");
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
