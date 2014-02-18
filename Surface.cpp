#include "Surface.h"

Surface::Surface(Ogre::String nym, 
	Ogre::SceneManager* mgr, 
	Simulator* sim, 
	Ogre::Real x, 
	Ogre::Real y, 
	Ogre::Real z, 
	Ogre::Real width, 
	Ogre::Real height, 
	Ogre::Real depth) 
: GameObject(nym, mgr, sim) 
{
    //setup Ogre
	//Ogre::Box surf();
	geom = mgr->createEntity(nym, "cube.mesh");

    //Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //sceneFloor, width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

    //Ogre::Entity* entFloor = mgr->createEntity("FloorEntity", "floor");
    //mgr->getSceneNode(nym)->createChildSceneNode("FloorNode")->attachObject(entFloor);

    geom->setMaterialName("Examples/Rockwall");
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