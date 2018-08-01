#include "Target.h"
#include "Game.h"


Target::Target() :
    startingRadius(6.0f), remainingRadiusToGrow(startingRadius),
    GameObject(
        rand() % (TheGame::Instance()->getWindowWidth()-30) +15,//x
        rand() % (TheGame::Instance()->getWindowHeight()-30) +15,//y
        0.0f,//starting radius of 0, will grow to remainingRadiusToGrow
        0,//height (not used)
        255,//r
        255,//g
        255,//b
        255 //a
               )
{
  
}

void Target::draw()
{
  filledCircleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), width, colourR, colourG, colourB, colourA);
  circleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), width, 0, 0, 0, 255);

}

void Target::update()
{
  if (remainingRadiusToGrow > 0)
  {
    remainingRadiusToGrow -= startingRadius/100;
    GameObject::width += startingRadius/100;
  }
}

void Target::clean()
{
  
}


void Target::consumedByAgent()
{
  //reduce the size of the target
  GameObject::width -= 0.22f;

  //if the target has been entirely consumed
  if (GameObject::width <= 0.2f)
  {
    respawn();
  }
}

void Target::respawn()
{
  //set a new randomised position, checking
  do {
    setNewRandomisedPosition();
  } while(!(TheEnvironment::Instance()->grid.getNodeFromCoords(position)->traversable));

  //set the width to 0, and reset the remainingRadiusToGrow so the fod will grow to its default size
  width = 0;//width
  remainingRadiusToGrow = startingRadius;

  colourR = 255;//r
  colourG = 255;//g
  colourB = 255;//b
}
