#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "PlayingField.h"

PlayingField::PlayingField(Ogre::SceneManager* scnMgr) {
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("PlayingField");

    Ogre::SceneNode* wallNodes[6];
    for (int i = 0; i < 1; i++) {
        std::ostringstream entityName;  
        entityName << "cube" << i;
        
        Ogre::Entity* wall = scnMgr->createEntity(entityName.str(), "cube.mesh");
        wall->setCastShadows(true);

        std::ostringstream nodeName;  
        nodeName << "wall" << i;

        wallNodes[i] = scnMgr->createSceneNode(nodeName.str());
        wallNodes[i]->attachObject(wall);
        rootNode->addChild(wallNodes[i]);
    }

    wallNodes[0]->scale(2.0f, 0.001f, 2.0f);
    wallNodes[0]->translate(Ogre::Vector3(0.0f, 0.0f, 0.0f));
}

void PlayingField::addChild(Ogre::SceneNode* child) {
    rootNode->addChild(child);
}

