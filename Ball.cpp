#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Ball.h"
#include "Target.h"

enum sounds{NOSOUND, BALLTARGET, BALLWALL, BALLPADDLE};

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
    if (mgr) {
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
    } else {
        // updateTransform aint gonna work so we have to set the transform ourselves
        tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    }

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
                simulator->soundPlayed = BALLTARGET;
            }

            Target* target = static_cast<Target*>(callback->ctxt.theObject);
            target->movePlacement();
        }
        else if (objName == "mypaddle") {                
            if (simulator->soundOn) {
                simulator->soundSystem->playRaquetHit();
                simulator->soundPlayed = BALLPADDLE;
            }
        }
        else if (objName == "mybox"){
            if (simulator->soundOn) {
                simulator->soundSystem->playWallHit();
                simulator->soundPlayed = BALLWALL;
            }
        }
        else {
            //nothing was hit
            if (simulator->soundOn) {
                simulator->soundPlayed = NOSOUND;
            }
        }
    }
}
