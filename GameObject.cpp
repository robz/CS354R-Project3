#include "GameObject.h"
#include "Simulator.h"

GameObject::GameObject(Ogre::String nym, Ogre::SceneManager* mgr, Simulator* sim){
	name = nym;
	sceneMgr = mgr;
	simulator = sim;
	rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
	shape = NULL;
	tr.setIdentity();
	mass = 0.0f;
	inertia.setZero();
}

void GameObject::updateTransform() {
	Ogre::Vector3 pos = rootNode->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = rootNode->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
	if(motionState)
		motionState->updateTransform(tr);
}

void GameObject::addToSimulator() {
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	updateTransform();
	motionState = new OgreMotionState(tr, rootNode);
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	if (mass != 0.0f) 
		shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
	body = new btRigidBody(rbInfo);
	simulator->addObject(this);
}

btRigidBody* GameObject::getBody(){
	return this->body;
}