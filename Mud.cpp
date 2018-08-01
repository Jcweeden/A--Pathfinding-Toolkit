#include "Mud.h"

Mud::Mud() :
    GameObject(
        rand() % (TheGame::Instance()->getWindowWidth()-30) + 15, //x
        rand() % (TheGame::Instance()->getWindowHeight()-30) + 15,//y
        rand() % 200 + 100,//width
        rand() % 200 + 100,//height
        96,//r
        70,//g
        15,//b
        255 //a
               )
   
{
  if (getPosition().getX() + width > TheGame::Instance()->getWindowWidth())
    width = TheGame::Instance()->getWindowWidth() - getPosition().getX() - 1;

  if (getPosition().getY() + height  > TheGame::Instance()->getWindowHeight())
    height = TheGame::Instance()->getWindowHeight() - getPosition().getY() - 1;
}


void Mud::draw()
{
  //draw mud itself
  boxRGBA(TheGame::Instance()->getRenderer(),
            getPositionX(), getPositionY(),
            getPositionX() + width, getPositionY() + height,
            colourR, colourG, colourB, colourA);
  /*
  //outline of box
  rectangleRGBA(TheGame::Instance()->getRenderer(),
                getPositionX(), getPositionY(),
                getPositionX() + width, getPositionY() + height,
                0, 0, 0, 255);
  */
}

void Mud::update()
{}

void Mud::clean()
{

}
