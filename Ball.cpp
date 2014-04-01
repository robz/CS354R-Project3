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
	nym = nym;
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
    score = 0;
}

void Ball::update() {
    if (callback->ctxt.hit) {
        Ogre::String& objName = callback->ctxt.theObject->name;

        if (objName == "mytarget") {
			simulator->soundPlayed = BALLTARGET;
            if (simulator->soundOn) {
                simulator->soundSystem->playTargetHit();
            }
            score++;
            Target* target = static_cast<Target*>(callback->ctxt.theObject);
            target->movePlacement();
        }
        else if (objName == "clientpaddle") {
			simulator->soundPlayed = CLIENTBALLPADDLE;
            if (simulator->soundOn) {
                simulator->soundSystem->playP2Hit();
            }
		}
		else if (objName == "serverpaddle") {
			simulator->soundPlayed = SERVERBALLPADDLE;
			if (simulator->soundOn) {
				simulator->soundSystem->playRaquetHit();
        	}
		}
        else if (isBox(objName)){
			simulator->soundPlayed = (this->nym == "serverball") ? BALL1WALL : BALL2WALL;
            if (simulator->soundOn) {
				int b = (this->nym == "serverball") ? 1 : 2;
                simulator->soundSystem->playWallHit(b);
            }
        }
    }
    else {
        //nothing was hit
    }
}

int& Ball::getScore(){
    return score;
}

void Ball::stepParticle(Ogre::ParticleSystem* ps){

   /*const float maxDist  = 250.0; 
   const float mirrorDist = maxDist*0.99; 
   const float dimFactor = 0.8*0.005*0.005;    
   const float maxDist2 = maxDist*maxDist; 
   const Ogre::Vector3& ballPos = rootNode->getPosition(); 
 
   Ogre::ParticleIterator pit = ps->_getIterator(); 
 
   while (!pit.end()) 
   {
      printf("Updating particle\n");
      Ogre::Particle* particle = pit.getNext(); 
      Ogre::Vector3& pos = particle->position; 
      particle->timeToLive = 999999.0f; 
      Ogre::Vector3 pDir = pos-ballPos; 
      float dist = pDir.squaredLength(); 
      float dim = dist*dimFactor; 
      particle->setDimensions(dim, dim); 
 
      if (dist > maxDist2) { 
         pDir.normalise(); 
         Ogre::Vector3 p = ballPos-pDir*mirrorDist;       
         particle->position = p; 
      } 
   }*/
}

void Ball::attachParticle(Ogre::ParticleSystem* ps){
    rootNode->attachObject(ps);
}
