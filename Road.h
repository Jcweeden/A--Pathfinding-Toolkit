#ifndef _Road_
#define _Road_

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h" 

#include "GameObject.h"
#include "Environment.h"
#include <iostream>
#include <string>

class GameObject;

class Road : public GameObject {
public:

  //no fields
public:
  Road(); //constructor
    
  void draw();
  void update();
  void clean();

};

#endif
