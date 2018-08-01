#include "Road.h"

Road::Road() :
    GameObject(
        rand() % (TheGame::Instance()->getWindowWidth()-30) + 15, //x
        rand() % (TheGame::Instance()->getWindowHeight()-30) + 15,//y
        rand() % 200 + 100,//width
        rand() % 200 + 100,//height
        192,//r
        192,//g
        192,//b
        255 //a
               )
   
{
    if (getPosition().getX() + width > TheGame::Instance()->getWindowWidth())
    width = TheGame::Instance()->getWindowWidth() - getPosition().getX() - 1;

  if (getPosition().getY() + height  > TheGame::Instance()->getWindowHeight())
    height = TheGame::Instance()->getWindowHeight() - getPosition().getY() - 1;
}


void Road::draw()
{
  //draw Road itself
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

void Road::update()
{}

void Road::clean()
{

}
