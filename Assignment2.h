/*
-----------------------------------------------------------------------------
Filename:    Assignment2.h
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
#ifndef __Assignment2_h_
#define __Assignment2_h_

#include <btBulletDynamicsCommon.h>
#include "BaseApplication.h"
#include "Ball.h"
#include "Box.h"
#include "Simulator.h"
#include "Surface.h"
#include "Wall.h"
#include "Target.h"

class Assignment2 : public BaseApplication
{
protected:
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

public:
    Assignment2(void);
    virtual ~Assignment2(void);

protected:
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // #ifndef __Assignment2_h_
