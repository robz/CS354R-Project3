
#ifndef __GameObject_h_
#define __GameObject_h_

#include <btBulletDynamicsCommon.h>
#include "OgreMotionState.h"
#include <Ogre.h>
#include "ContactSensorCallback.h"

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
		Ogre::Real restitution;
    	Ogre::Real friction;
	public:
        ContactSensorCallback* callback;
 
		btCollisionShape* shape;
		GameObject(Ogre::String, Ogre::SceneManager*, Simulator*, Ogre::Real, Ogre::Real);
		void updateTransform();
		void addToSimulator();
		btRigidBody* getBody();
		void move(Ogre::Real, Ogre::Real, Ogre::Real);
		void rotate(Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Node::TransformSpace val_x = Ogre::Node::TS_LOCAL, Ogre::Node::TransformSpace val_y = Ogre::Node::TS_LOCAL, Ogre::Node::TransformSpace val_z = Ogre::Node::TS_LOCAL);
		Ogre::SceneNode& getNode();
		void setKinematic();

        virtual void update();
}; 

#endif
