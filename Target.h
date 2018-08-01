#ifndef _Target_
#define _Target_

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h" 
//#include "Vector2D.h"
#include "GameObject.h"
#include "Environment.h"


#include <iostream>
#include <string>

class GameObject;

class Target : public GameObject {
public:
  //the default radius of a target, that it will be spawned and respawned to be at
  float startingRadius;

  //the amount of radi left to expand - value is decremented each frame and added to size of the circle until
  //it has fully grown
  float remainingRadiusToGrow;

public:

  Target(); //const

    
  void draw();
  void update();
  void clean();

  //when in contact with a agent the target is consumed and it's size reduced
  void consumedByAgent();

  //upon being consumed the target is relocated to a new position and has remainingRadiusToGrow reset so it
  //will expand to full size again
  void respawn();
};

#endif
