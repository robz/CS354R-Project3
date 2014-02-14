#ifndef __ball__
#define __ball__

#include <Ogre.h>

class Ball : public GameObject {
public:
    Ball(
        Ogre::String nym, 
        Ogre::SceneManager* mgr, 
        Simulator* sim, 
        float radius, 
        float m, 
        Ogre::Vector3 pos, 
        float restitution, 
        float friction
        );
};

#endif
