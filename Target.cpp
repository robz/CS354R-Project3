#include <iostream>
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
    circle->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);

    float accuracy = 30;
    const float thickness = radius/4;
    unsigned int index = 0;
     
    for(float theta = 0; theta <= 2 * Ogre::Math::PI; theta += Ogre::Math::PI / accuracy) 
    {
        circle->position(radius * cos(theta),
                         0,
                         radius * sin(theta));
        circle->position(radius * cos(theta - Ogre::Math::PI / accuracy),
                         0,
                         radius * sin(theta - Ogre::Math::PI / accuracy));
        circle->position((radius - thickness) * cos(theta - Ogre::Math::PI / accuracy),
                         0,
                         (radius - thickness) * sin(theta - Ogre::Math::PI / accuracy));
        circle->position((radius - thickness) * cos(theta),
                         0,
                         (radius - thickness) * sin(theta));
        circle->quad(index, index + 1, index + 2, index + 3);
        index += 4;
    }
    circle->end();
    
    rootNode->attachObject(circle);

    shape = new btCylinderShape(btVector3(radius, radius, .01)); 

    wall = 0;
    setPose(wall++, 0, 0);
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

    z -= boxHeight/2.0;

    this->move(x, y, z);
    this->rotate(yaw, pitch, 0);
}

void Target::update() {
    if (callback->ctxt.hit && callback->ctxt.theObject->name == "myball") {
        std::cout << "hit target: " << wall << std::endl;
        setPose((wall++)%6, 0, 0);
    }
}

