#include "Box.h"

Box::Box(Ogre::String nym, Ogre::SceneManager* mgr, Simulator* sim, Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real width, Ogre::Real height, Ogre::Real depth) {
    mgr->getRootSceneNode()->createChildSceneNode(nym);
    mgr->getSceneNode(nym)->setPosition(x, y, z);
    //setup Ogre
	Ogre::Plane sceneFloor(Ogre::Vector3::UNIT_Y, 0);

    Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    sceneFloor, width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

    Ogre::Entity* entFloor = mgr->createEntity("FloorEntity", "floor");
    mgr->getSceneNode(nym)->createChildSceneNode("FloorNode")->attachObject(entFloor);

    entFloor->setMaterialName("Examples/Rockwall");
    entFloor->setCastShadows(false);

    wall[0] = new GameObject("FloorNode", mgr, sim);
    wall[0]->shape = new btStaticPlaneShape(btVector3(0,1,0),1);

    Ogre::Plane sceneCeiling(-Ogre::Vector3::UNIT_Y, 0);

    Ogre::MeshManager::getSingleton().createPlane("Ceiling", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    sceneCeiling, width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

    Ogre::Entity* entCeiling = mgr->createEntity("CeilingEntity", "Ceiling");
    mgr->getSceneNode(nym)->createChildSceneNode("CeilingNode", Ogre::Vector3( 0, height, 0 ))->attachObject(entCeiling);

    entCeiling->setMaterialName("Examples/Rockwall");
    entCeiling->setCastShadows(false);

    wall[1] = new GameObject("CeilingNode", mgr, sim);
    wall[1]->shape = new btStaticPlaneShape(btVector3(0,-1,0),1);

    Ogre::Plane sceneWallPosX(Ogre::Vector3::UNIT_X, 0);

    Ogre::MeshManager::getSingleton().createPlane("wallPosX", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    sceneWallPosX, width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);

    Ogre::Entity* entWallPosX = mgr->createEntity("WallPosXEntity", "wallPosX");
    mgr->getSceneNode(nym)->createChildSceneNode("WallPosXNode", Ogre::Vector3( -width/2, height/2, 0))->attachObject(entWallPosX);

    entWallPosX->setMaterialName("Examples/Rockwall");
    entWallPosX->setCastShadows(false);

    wall[2] = new GameObject("WallPosXNode", mgr, sim);
    wall[2]->shape = new btStaticPlaneShape(btVector3(1,0,0),1);

    Ogre::Plane sceneWallNegX(-Ogre::Vector3::UNIT_X, 0);

    Ogre::MeshManager::getSingleton().createPlane("wallNegX", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    sceneWallNegX, width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);

    Ogre::Entity* entWallNegX = mgr->createEntity("WallNegXEntity", "wallNegX");
    mgr->getSceneNode(nym)->createChildSceneNode("WallNegXNode", Ogre::Vector3( width/2, height/2, 0))->attachObject(entWallNegX);

    entWallNegX->setMaterialName("Examples/Rockwall");
    entWallNegX->setCastShadows(false);

    wall[3] = new GameObject("WallNegXNode", mgr, sim);
    wall[3]->shape = new btStaticPlaneShape(btVector3(-1,0,0),1);

    Ogre::Plane sceneWallPosZ(Ogre::Vector3::UNIT_Z, 0);

    Ogre::MeshManager::getSingleton().createPlane("wallPosZ", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    sceneWallPosZ, width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);

    Ogre::Entity* entWallPosZ = mgr->createEntity("WallPosZEntity", "wallPosZ");
    mgr->getSceneNode(nym)->createChildSceneNode("WallPosZNode", Ogre::Vector3( 0, height/2, -depth/2))->attachObject(entWallPosZ);

    entWallPosZ->setMaterialName("Examples/Rockwall");
    entWallPosZ->setCastShadows(false);

    wall[4] = new GameObject("WallPosZNode", mgr, sim);
    wall[4]->shape = new btStaticPlaneShape(btVector3(0,0,1),1);

    Ogre::Plane sceneWallNegZ(-Ogre::Vector3::UNIT_Z, 0);

    Ogre::MeshManager::getSingleton().createPlane("wallNegZ", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    sceneWallNegZ, width, height, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);

    Ogre::Entity* entWallNegZ = mgr->createEntity("WallNegZEntity", "wallNegZ");
    mgr->getSceneNode(nym)->createChildSceneNode("WallNegZNode", Ogre::Vector3( 0, height/2, depth/2))->attachObject(entWallNegZ);

    entWallNegZ->setMaterialName("Examples/Rockwall");
    entWallNegZ->setCastShadows(false);

    wall[5] = new GameObject("WallNegZNode", mgr, sim);
    wall[5]->shape = new btStaticPlaneShape(btVector3(0,0,-1),1);

    //setup bullet
    //shape = new btBoxShape(btVector3(width,height,depth));

}

void Box::addToSimulator(){
    for(int i = 0; i < 6; i++){
        wall[i]->addToSimulator();
    }
}
