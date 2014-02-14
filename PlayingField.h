// PlayingField class, inspired by interface presented in class

#ifndef __playing__
#define __playing__

#include <Ogre.h>

class PlayingField {
protected:
    Ogre::SceneNode* rootNode;
    
public:
    PlayingField(Ogre::SceneManager* scnMgr);
    ~PlayingField();
    void addChild(Ogre::SceneNode* child);
};

#endif

