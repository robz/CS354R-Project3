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
    Ogre::Real restitution,
    Ogre::Real friction,
    Ogre::String tex = "") 
: GameObject(nym, mgr, sim, restitution, friction) 
{
    //setup Ogre
    geom = mgr->createEntity(nym, "cube.mesh");
    if(tex != "")
        geom->setMaterialName(tex);
    geom->setCastShadows(false);
    Ogre::MaterialPtr mat = geom->getSubEntity(0)->getMaterial();
    /*Ogre::MaterialPtr alphamat = Ogre::MaterialManager::getSingleton().create("testy", 
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);*/
    Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
    pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    pass->setDepthWriteEnabled(false);
    Ogre::TextureUnitState* t = pass->createTextureUnitState();
    t->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, 0.7);

    rootNode->attachObject(geom);
    rootNode->setPosition(x, y, z);
    rootNode->scale(
        width * 0.01f,
        height * 0.01f,
        depth * 0.01f
        );
    shape = new btBoxShape(btVector3(width/2.0,height/2.0,depth/2.0));
}
/*
void Surface::update() {
    bool wasHit = callback->ctxt.hit;
    
    if (wasHit) {
        // simulator->playHitSound();
    }
}
*/
