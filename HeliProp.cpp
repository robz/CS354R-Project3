#include "HeliProp.h"

HeliProp::HeliProp(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim,
    Ogre::Real scale, 
    Ogre::Real m, 
    Ogre::Vector3 pos, 
    Ogre::Real restitution, 
    Ogre::Real friction,
    Ogre::String tex = ""
    ) 
: GameObject(nym, mgr, sim, restitution, friction)
{
    if (mgr) {
        geom = mgr->createEntity("heliPropEnt", "heliprop.mesh");
        if(tex != "")
            geom->setMaterialName(tex);
        geom->setCastShadows(false);
        updateNode("heliProp");
        rootNode->attachObject(geom);

        // sphere starts at 100 units radius
        rootNode->scale(
            scale,
            scale,
            scale
            );

        rootNode->setPosition(pos);
    } else {
        // updateTransform aint gonna work so we have to set the transform ourselves
        tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    }

    //need to figure this out
    shape = new btBoxShape(btVector3(scale*8.374/2, scale*3.112/2, scale*8.358/2));
    mass = m;
}

void HeliProp::updateNode(Ogre::String n){
    rootNode = rootNode->createChildSceneNode(n);
    name = n;
}

void HeliProp::spin(Ogre::Real t){
    rotate(t * 1420, 0.0, 0.0);
}

void HeliProp::updateTransform() {
    if (!rootNode) return;

    Ogre::Vector3 pos = rootNode->getParentSceneNode()->getPosition();
    tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    Ogre::Quaternion qt = rootNode->getOrientation();
    tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
    
    if(motionState){
        motionState->updateTransform(tr);
    }
}