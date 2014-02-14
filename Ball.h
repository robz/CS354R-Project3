// ball class, inspired by code presented in class
#ifndef __ball__
#define __ball__

#include <Ogre.h>
#include "PlayingField.h"

class Ball {
protected:
    Ogre::SceneNode* rootNode;
    Ogre::Real radius;
    PlayingField* field;

public:
    Ball(Ogre::SceneManager* scnMgr);
    ~Ball();
    Ogre::SceneNode* getNode();
    void setPlayingField(PlayingField * f);
};

#endif
