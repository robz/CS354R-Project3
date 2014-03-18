#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Ball.h"
#include "Target.h"

Ball::Ball(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim,
    Ogre::Real radius, 
    Ogre::Real m, 
    Ogre::Vector3 pos, 
    Ogre::Real restitution, 
    Ogre::Real friction,
    Ogre::String tex = ""
    ) 
: GameObject(nym, mgr, sim, restitution, friction)
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

void Ball::update() {
    if (callback->ctxt.hit) {
        Ogre::String& objName = callback->ctxt.theObject->name;

        if (objName == "mytarget") {
            if (simulator->soundOn) {
                simulator->soundSystem->playTargetHit();
            }

            Target* target = static_cast<Target*>(callback->ctxt.theObject);
            target->movePlacement();
        }
        else if (objName == "mypaddle") {                
            if (simulator->soundOn) {
                simulator->soundSystem->playRaquetHit();
            }
        }
        else {
            if (simulator->soundOn) {
                simulator->soundSystem->playWallHit();
            }
        }
    }
}
