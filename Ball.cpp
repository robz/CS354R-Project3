#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Ball.h"

Ball::Ball(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim,
    float radius, 
    float m, 
    Ogre::Vector3 pos, 
    float restitution, 
    float friction,
    Ogre::String tex = ""
    ) 
: GameObject(nym, mgr, sim)
{
    geom = mgr->createEntity(name, "sphere.mesh");
    if(tex != "")
        geom->setMaterialName(tex);
    geom->setCastShadows(false);
    rootNode->attachObject(geom);

    // sphere starts at 100 units radius
    rootNode->scale(
        radius * 0.01f,
        radius * 0.01f,
        radius * 0.01f
        );

    rootNode->setPosition(pos);

    shape = new btSphereShape(radius);
    mass = m;
    hitFlag = false;
}
/*
void Ball::update() {
    bool wasHit = callback->ctxt.hit;
    
    if (wasHit) {
        // simulator->playHitSound();
    }
}
*/
