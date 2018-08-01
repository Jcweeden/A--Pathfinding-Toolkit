#ifndef _IGameObject_
#define _IGameObject_
#include "SDL2/SDL.h"
#include <iostream>
#include <string>


class I_GameObject {

public:

  I_GameObject() {}
  
  virtual void draw() = 0;
  virtual void update() = 0;
  virtual void clean() = 0;

protected:

  
  //virtual ~I_GameObject() {}

};

#endif
