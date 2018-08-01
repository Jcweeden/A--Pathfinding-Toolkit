#include "GameObject.h"

GameObject::GameObject( int x, int y, int p_width, int p_height,  Uint8 p_colourR, Uint8 p_colourG, Uint8 p_colourB, Uint8 p_colourA) :
    position(x,y), width(p_width), height(p_height), velocity(0,0), acceleration(0,0), damping(0.0f), colourR(p_colourR), colourG(p_colourG), colourB(p_colourB), colourA(p_colourA), forceAccumulated(0,0)
{
}

void GameObject::draw()
{
}

void GameObject::update()
{
  velocity += acceleration;
  position += velocity;

  physicsIntegration();

};

void GameObject::clean()
{
}


//calculate and apply physics forces to the object should it have mass
void GameObject::physicsIntegration()
{
  //ensure has mass
  if (inverseMass <= 0.0f)
  {
    //std::cout << "GameObject.cpp.Integration() - inverseMass below 0\n";
    return;
  }
    
  float duration = (float)TheGame::Instance()->getFrameTime() * (float)0.01f;
  //std::cout << duration << "\n";
  if (duration <= 0.0)
  {
    //std::cout << "ShapeBody.cpp.Integration() - duration below 0\n";
    return;
  }

  
  //update position
  position.addScaledVector(velocity, duration);

  //calc acceleration
  Vector2D resultingAcceleration = acceleration;
  resultingAcceleration.addScaledVector(forceAccumulated, inverseMass);

  //update velocity from acceleration
  velocity.addScaledVector(resultingAcceleration, duration);

  //add drag
  velocity *= powf(damping, duration);
  
  clearAccumForces();
}


void GameObject::setMass(const float mass)
{
  if(mass == 0)
  {
    std::cout << "GameObject.cpp.setMass() - requires mass not of 0\n";
    return;
  } else
  {
    inverseMass = ((float)1.0f)/mass;
  }
}


//adds force to the vector that will be applied at the end of each frame
void GameObject::addForce(const Vector2D &force)
{
  forceAccumulated += force;
}

void GameObject::clearAccumForces()
{
  forceAccumulated.clear();
}

bool GameObject::checkForCollisionWithCircle(GameObject* obj)
{
  //sqrt(sqr(x)+sqr(y))
  
  float dx = obj->getPositionX() - position.getX();
  float dy = obj->getPositionY() - position.getY();
  float radii = obj->getWidth() + getWidth();

  //if distance between origins of 2 circles is smaller than their combined radii 
  if ( ( dx * dx )  + ( dy * dy ) < radii * radii )   {
    //the circles are colliding
    return true;
  }
  else
  {
    return false;
  }
}

bool GameObject::checkForCollisionWithPoint(Vector2D* point)
{
  //sqrt(sqr(x)+sqr(y))
  
  float dx = point->getX() - position.getX();
  float dy = point->getY() - position.getY();
  float radii = /*point->getWidth()*/1 + getWidth();

  //if distance between origins of 2 circles is smaller than their combined radii 
  if ( ( dx * dx )  + ( dy * dy ) < radii * radii )   {
    //the circles are colliding
    return true;
  }
  else
  {
    return false;
  }
}

void GameObject::setNewRandomisedPosition()
{
  setPosition(Vector2D(rand() % (TheGame::Instance()->getWindowWidth()-30) + 15, //x
                       rand() % (TheGame::Instance()->getWindowHeight()-30) + 15));//y
} 

