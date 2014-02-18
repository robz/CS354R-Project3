
#ifndef __GameObject_h_
#define __GameObject_h_

#include <btBulletDynamicsCommon.h>
#include "OgreMotionState.h"
#include <Ogre.h>

class Simulator;

class GameObject {
	protected:
		Ogre::String name;
		Ogre::SceneManager* sceneMgr;
		Simulator* simulator;
		Ogre::SceneNode* rootNode;
		Ogre::Entity* geom;
		btScalar mass;
		btRigidBody* body;
		btTransform tr;
		btVector3 inertia;
		OgreMotionState* motionState;
	public:
		btCollisionShape* shape;
		GameObject(Ogre::String, Ogre::SceneManager*, Simulator*);
		void updateTransform();
		void addToSimulator();
		btRigidBody* getBody();
		void move(Ogre::Real, Ogre::Real, Ogre::Real);
}; 

#endif