#ifndef _Mud_
#define _Mud_

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h" 

#include "GameObject.h"
#include "Environment.h"
#include <iostream>
#include <string>

class GameObject;

class Mud : public GameObject {
public:

  //no fields
public:
  Mud(); //constructor
    
  void draw();
  void update();
  void clean();

};

#endif
