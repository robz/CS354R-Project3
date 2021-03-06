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

float indoffset = .5;

Ogre::Vector3 indPoints[7] = {
    Ogre::Vector3(0.0+indoffset, 0.05, 0.0),
    Ogre::Vector3(0.3+indoffset, 0.05, 0.0),
    Ogre::Vector3(0.3+indoffset, 0.1, 0.0),
    Ogre::Vector3(0.4+indoffset, 0.0, 0.0),
    Ogre::Vector3(0.3+indoffset, -0.1, 0.0),
    Ogre::Vector3(0.3+indoffset, -0.05, 0.0),
    Ogre::Vector3(0.0+indoffset, -0.05, 0.0)
};

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
	gui = new GUI(spSub, clientSub, serverSub);
	   
	// Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    // Create a manual object for 2D
    manualInd = mSceneMgr->createManualObject("manual");
     
    // Use identity view/projection matrices
    manualInd->setUseIdentityProjection(true);
    manualInd->setUseIdentityView(true);
     
    manualInd->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

    for (int i = 0; i < 7; i++) {
        Ogre::Vector3 point = indPoints[i];
        point.z = -1000;
        manualInd->position(point);
    }
    for (int i = 0; i < 7; i++) {
        manualInd->index(i);
    }
    manualInd->index(0);
     
    manualInd->end();
     
    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manualInd->setBoundingBox(aabInf);
     
    // Render just before overlays
    manualInd->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
     
    // Attach to scene
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(manualInd);

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(10.0f, 10.0f, 10.0f);
}

float PADDLE_X_SPEED = 60.0f,
      PADDLE_Y_SPEED = 60.0f,
      PADDLE_Z_SPEED = 60.0f,
      PADDLE_ROT_SPEED = 30.0f,
      HELI_SPEED = 120.0f;

bool Assignment3::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    
    static Ogre::Real z_time = 0.0;
    //static Ogre::Real heli_time = 0.0;
    //static bool heli_init = false;
    //static int rotate_time = 0;

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

        updateIndicator((isClient) ? clientBall : serverBall);
        
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

                //update helicopter from server
                float* heliPose = servData.getHeli();
                heli->getNode().setPosition(heliPose[0], heliPose[1], heliPose[2]);
                heli->setPropRot(heliPose[3], heliPose[4], heliPose[5], heliPose[6]);
                heli->updateTransform();

                //play sounds (if any)
               if(simulator->soundOn){
                    int sound = servData.getSound();
                    if(sound == BALL1WALL) {
                        simulator->soundSystem->playWallHit(1);
                    } else if (sound == BALL2WALL) {
						simulator->soundSystem->playWallHit(2);
					} else if(sound == BALLTARGET) {
                        simulator->soundSystem->playTargetHit();
                    } else if(sound == CLIENTBALLPADDLE) {
                        simulator->soundSystem->playP2Hit();
					} else if (sound == SERVERBALLPADDLE) {
						simulator->soundSystem->playRaquetHit();
                    } else if (sound == NOSOUND) {
						simulator->soundSystem->resetFlags();
                    } else {
                        std::cout << "WHAT THE FUCK SOUND IS THAT" << std::endl;
                    }
                }

                //update score
                int* score = servData.getScore();
                gui->setMultiplayerScores(score[0], score[1]);
			}
    
            // send the state of our paddle to the server
            clientData.paddlePos = clientPaddle->getNode().getPosition();
            clientData.paddleOrient = clientPaddle->getNode().getOrientation();
            client->sendMsg(reinterpret_cast<char*>(&clientData), sizeof(clientData));
            clientData.gravityChange = 0;
        } else {

            heli->animate(evt.timeSinceLastFrame);
            
            if(!isSinglePlayer){
                //btTransform trans; 
                server->awaitConnections();
                // step the server's simulator
                simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);
                // send the state of the target to the client
                ServerToClient* data = initServerToClient();
                server->sendMsg(reinterpret_cast<char*>(data), sizeof(ServerToClient));
                simulator->soundPlayed = NOSOUND;
                delete data;
            
                // get the state of the paddle from the client
                ClientToServerData cdata;
                if (server->recMsg(reinterpret_cast<char*>(&cdata))) {
                    clientPaddle->getNode().setPosition(cdata.paddlePos);
                    clientPaddle->getNode().setOrientation(cdata.paddleOrient);
                    clientPaddle->updateTransform();
                    simulator->setGravity(simulator->gravity + cdata.gravityChange); 
                }
            }
            else {
                simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1/60.0f);
            } 
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

void Assignment3::updateIndicator(Ball* ball) {
    Ogre::Vector3 world_point = ball->getNode().getPosition();

    bool isBallVisible = mCamera->isVisible(Ogre::Sphere(world_point, 1.0));
    if (!isBallVisible) {
        Ogre::Vector3 screen_point = mCamera->getProjectionMatrix() * (mCamera->getViewMatrix() * world_point);  
        
        float angle = atan2(screen_point.x, screen_point.y);

        manualInd->beginUpdate(0);
        Ogre::Quaternion rot(Ogre::Radian(-angle + M_PI/2.0), Ogre::Vector3::UNIT_Z);
        for (int i = 0; i < 7; i++) {
            manualInd->position(rot * indPoints[i]);
        }
        manualInd->end();
    } else {
        manualInd->beginUpdate(0);
        for (int i = 0; i < 7; i++) {
            Ogre::Vector3 point = indPoints[i];
            point.z = -1000;
            manualInd->position(point);
        }
        manualInd->end();
    }
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

    //helicopter information
    float heliPose[7];
    heliPose[0] = heli->getNode().getPosition().x;
    heliPose[1] = heli->getNode().getPosition().y;
    heliPose[2] = heli->getNode().getPosition().z;
    heliPose[3] = heli->getProp()->getNode().getOrientation().w;
    heliPose[4] = heli->getProp()->getNode().getOrientation().x;
    heliPose[5] = heli->getProp()->getNode().getOrientation().y;
    heliPose[6] = heli->getProp()->getNode().getOrientation().z;

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
    data->setHeli(heliPose);

    return data;
}

bool Assignment3::keyPressed(const OIS::KeyEvent &arg)
{   
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	sys.injectChar(arg.text);

	if (simulator) {
			if (arg.key == OIS::KC_R) {
		        if (isClient) {
                    clientData.gravityChange = 20; 
                } else {
                    simulator->setGravity(simulator->gravity + 20.0); 
			    }
            } else if (arg.key == OIS::KC_F) {
			    if (isClient) {
                    clientData.gravityChange = -20; 
                } else {
            		simulator->setGravity(simulator->gravity - 20.0);
			    }
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

void Assignment3::createSceneObjects() {
    srand (time(NULL));

    box = new Box("mybox", mSceneMgr, simulator, 0, 0, 0, 150.0, 150.0, 150.0, 0.9, 0.1, "Examples/Rockwall", "Examples/BeachStones");
    target = new Target("mytarget", mSceneMgr, simulator, 0, 0, 0, 130, 130, 130, 20);
    target->setPose(startingFace, 0, 0);
    
    serverBall = new Ball("serverball", mSceneMgr, simulator, 1.0, 1.0, 
        Ogre::Vector3(rand()%100 - 50, rand()%100+25, rand()%100 - 50), 
        .9f, .1f, "Game/P1ball");
    serverPaddle = new Surface("serverpaddle", mSceneMgr, simulator, 
            rand()%100 - 50, rand()%100+25, rand()%100 - 50,
            10.0, 10.0, 2.5, 0.25, 0.1, "Game/P1paddle");
		
    if (!isSinglePlayer) {
        clientBall = new Ball("clientball", mSceneMgr, simulator, 1.0, 1.0, 
            Ogre::Vector3(rand()%100 - 50, rand()%100+25, rand()%100 - 50), 
            .9f, .1f, "Game/P2ball");
		clientPaddle = new Surface("clientpaddle", mSceneMgr, simulator, 
            rand()%100 - 50, rand()%100+25, rand()%100 - 50,
            10.0, 10.0, 2.5, 0.25, 0.1, "Game/P2paddle");
    }

    heli = new Heli("dachoppa", mSceneMgr, simulator, 3.0, 1.0, Ogre::Vector3(0.0, 0.0, 45.0), 0.9, 0.1, "Game/Helicopter");
}

bool Assignment3::singlePlayer(const CEGUI::EventArgs &e)
{
    isClient = false;
    isSinglePlayer = true;

    simulator = new Simulator();
 
    // Create a scene
    createSceneObjects();   
 
    //Setup player camera
    (&(serverPaddle->getNode()))->createChildSceneNode("camNode");
    mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);
    
    box->addToSimulator();
    target->addToSimulator();
    target->setKinematic();
    
    serverBall->addToSimulator();
    serverPaddle->addToSimulator();
    serverPaddle->setKinematic();

    heli->addToSimulator();
    heli->setKinematic();
    
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

	if (client->serverFound) {
		simulator = new Simulator();

		// Create a scene
        createSceneObjects();

		//Setup player camera
		(&(clientPaddle->getNode()))->createChildSceneNode("camNode");
		mSceneMgr->getSceneNode("camNode")->attachObject(mCamera);
		gui->destroyMenu(false);
		gameplay = true;
	}
	return true;
}

bool Assignment3::serverStart(const CEGUI::EventArgs &e)
{
	isClient = false;
	sPort = gui->getPort();
    server = new Server(sPort);
	
    simulator = new Simulator();
  
    // Create a scene
    createSceneObjects();   
 
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

    heli->addToSimulator();
    heli->setKinematic();
    
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
