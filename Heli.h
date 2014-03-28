#ifndef __heli__
#define __heli__

#include "Simulator.h"
#include <Ogre.h>

class Heli : public GameObject {
protected:
public:
    Heli(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        Ogre::Real scale, 
        Ogre::Real m, 
        Ogre::Vector3 pos, 
        Ogre::Real restitution, 
        Ogre::Real friction,
        Ogre::String
        );
};

#endif