#ifndef __Box_h_
#define __Box_h_

#include "GameObject.h"
#include "Surface.h"

class Box {
	protected:
		GameObject* wall[6];

	public:
		Box(Ogre::String, Ogre::SceneManager*, Simulator*, Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Real, Ogre::Real);
		void addToSimulator();
};

#endif