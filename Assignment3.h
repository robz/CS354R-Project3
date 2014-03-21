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

#include <btBulletDynamicsCommon.h>
#include "BaseApplication.h"
#include "Ball.h"
#include "Box.h"
#include "Simulator.h"
#include "Surface.h"
#include "Wall.h"
#include "Target.h" 
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

class Assignment3 : public BaseApplication
{
protected:
    CEGUI::OgreRenderer* mRenderer;
    CEGUI::Window *p1score;
	CEGUI::Window *p2score;
	CEGUI::Window *singlebtn;
	CEGUI::Window *clientbtn;
	CEGUI::Window *serverbtn;
	CEGUI::Editbox *serverIP;
	CEGUI::Editbox *cServerPort;
	CEGUI::Editbox *cClientPort;
	CEGUI::Editbox *clientIP;
	CEGUI::Editbox *sServerPort;
	CEGUI::Editbox *sClientPort;
    Simulator* simulator;
    Ball* ball;
    Box* box;
    Surface* paddle;
    Target* target;
    
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
    Assignment3(void);
    virtual ~Assignment3(void);
};

#endif // #ifndef __Assignment3_h_
