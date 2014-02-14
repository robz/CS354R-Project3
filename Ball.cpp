#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Ball.h"

Ball::Ball(Ogre::SceneManager* scnMgr) {
    Ogre::Entity* ball = scnMgr->createEntity("Sphere", "sphere.mesh");
    ball->setMaterialName("BallColor/CubeMap");
    ball->setCastShadows(true);

    rootNode = scnMgr->createSceneNode("Ball");
    rootNode->attachObject(ball);
    rootNode->scale(0.1f,0.1f,0.1f); // sphere starts at 100 units radius

    radius = 10.0f;
}

Ogre::SceneNode* Ball::getNode() { 
    return rootNode; 
}

void Ball::setPlayingField(PlayingField * f) { 
    field = f; 
}

