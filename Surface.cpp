#include "Surface.h"

Surface::Surface(Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim, 
    Ogre::Real x, 
    Ogre::Real y, 
    Ogre::Real z, 
    Ogre::Real width, 
    Ogre::Real height, 
    Ogre::Real depth,
    Ogre::String tex = "") 
: GameObject(nym, mgr, sim) 
{
    //setup Ogre
    geom = mgr->createEntity(nym, "cube.mesh");
    if(tex != "")
        geom->setMaterialName(tex);
    geom->setCastShadows(false);

    rootNode->attachObject(geom);
    rootNode->setPosition(x, y, z);
    rootNode->scale(
        width * 0.01f,
        height * 0.01f,
        depth * 0.01f
        );
    shape = new btBoxShape(btVector3(width/2,height/2,depth/2));
}
/*
void Surface::update() {
    bool wasHit = callback->ctxt.hit;
    
    if (wasHit) {
        // simulator->playHitSound();
    }
}
*/
