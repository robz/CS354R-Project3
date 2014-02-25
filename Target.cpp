#include "Target.h"

Target::Target(
    Ogre::String nym, 
    Ogre::SceneManager* mgr, 
    Simulator* sim, 
    Ogre::Real boxX, 
    Ogre::Real boxY, 
    Ogre::Real boxZ, 
    Ogre::Real boxWidth, 
    Ogre::Real boxHeight, 
    Ogre::Real boxDepth,
    Ogre::Real radius
    )
: GameObject(nym, mgr, sim), radius(radius), boxWidth(boxWidth), boxHeight(boxHeight), boxDepth(boxDepth)
{
    Ogre::ManualObject* circle = mgr->createManualObject("Circle");
    circle->begin("BaseWhite", Ogre::RenderOperation::OT_LINE_STRIP);

    const float accuracy = 30;
    unsigned int index = 0;
    
    for(float theta = 0; theta <= 2 * Ogre::Math::PI; theta += Ogre::Math::PI / accuracy)
    {
       circle->position(cos(theta)*radius, 0, sin(theta)*radius);
       circle->index(index++);
    }
     
    circle->end();
    
    rootNode->attachObject(circle);

    shape = new btCylinderShape(btVector3(radius, radius, .1)); 

    setPose(0, 0, 0);
}

void Target::setPose(int wall, float xOffset, float yOffset) {
    float x, y, z, pitch = 0, yaw = 0;
    
    if (wall == 0 || wall == 1) {
        x = xOffset;
        y = yOffset;
        z = (wall == 0) ? boxDepth/2.0 : -boxDepth/2.0;
        pitch = M_PI/2.0;
    } else if (wall == 2 || wall == 3) {
        x = xOffset;
        y = (wall == 2) ? boxHeight/2.0 : -boxHeight/2.0;
        z = yOffset;
    } else if (wall == 4 || wall == 5) {
        x = (wall == 4) ? boxWidth/2.0 : -boxWidth/2.0;
        y = xOffset;
        z = yOffset;
        yaw = M_PI/2.0;
    }

    this->move(x, y, z);
    this->rotate(yaw, pitch, 0);
}

