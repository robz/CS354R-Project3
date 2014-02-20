#ifndef __ball__
#define __ball__

#include "Simulator.h"
#include <Ogre.h>

class Ball : public GameObject {
protected:
    bool hitFlag;

public:
    Ball(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        float radius, 
        float m, 
        Ogre::Vector3 pos, 
        float restitution, 
        float friction,
        Ogre::String
        );

    // void update();
};

#endif
