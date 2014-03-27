/*
-----------------------------------------------------------------------------
Filename:    Assignment3.h
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
#ifndef __Assignment3_h_
#define __Assignment3_h_

#include "UDPNetEnt.h"
#include <btBulletDynamicsCommon.h>
#include "BaseApplication.h"
#include "Ball.h"
#include "Box.h"
#include "Simulator.h"
#include "Surface.h"
#include "Wall.h"
#include "Target.h"
#include "Client.h"
#include "Server.h"
#include "GUI.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

class Assignment3 : public BaseApplication
{
protected:
	GUI* gui;
    CEGUI::OgreRenderer* mRenderer;
    CEGUI::Window *p1score;
	CEGUI::Window *p2score;
	CEGUI::Window *singlebtn;
	CEGUI::Window *clientbtn;
	CEGUI::Window *serverbtn;
	CEGUI::Editbox *serverIP;
	CEGUI::Editbox *serverPort;
    Simulator* simulator;
    Ball* ball;
    Box* box;
    Surface* paddle;
    Target* target;
    
    Server* server;
    Client* client;
    bool isClient;

    // OIS::KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    virtual void createScene(void);
	virtual void createFrameListener(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
 
	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
    bool singlePlayer(const CEGUI::EventArgs &e);
	bool clientStart(const CEGUI::EventArgs &e);
	bool serverStart(const CEGUI::EventArgs &e);
	void destroyMenu(void);
	char* CEGUIStringToString(CEGUI::String cestr);

public:
    Assignment3();
    virtual ~Assignment3(void);
};

#endif // #ifndef __Assignment3_h_
