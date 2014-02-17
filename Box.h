#ifndef __Box_h_
#define __Box_h_

#include "GameObject.h"

class Box : public GameObject {
	public:
		Box(Ogre::String, Ogre::SceneManager*, Simulator*, Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Real);
};

#endif