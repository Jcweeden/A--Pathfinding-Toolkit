#include "Agent.h"
#include <limits.h>


Agent::Agent() :
    GameObject(
        rand() % (TheGame::Instance()->getWindowWidth()-30) + 15, //x
        rand() % (TheGame::Instance()->getWindowHeight()-30) + 15,//y
        15.0f,//width (used as the radius)
        0,    //height (not used)
        255,//r
        255,//g
        255,//b
        rand() % 160 + 40 //a
               ),
    speedMultiplier(10.0f), startingRadius(15.0f), framesToNextPathfind(0), nearestTarget(-1)
{
}

void Agent::draw()
{
  //draw agent itself
  filledCircleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), width, colourR, colourG, colourB, colourA);
  circleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), width, 0, 0, 0, 255);
}

void Agent::update()
{
  //resetVal is how many frames should elapse between each update of running the pathfinding algorithm
  int resetVal = 4;

  //if off the screen
  if ((position.getX() < 0 || position.getX() > TheGame::Instance()->getWindowWidth() ||
       position.getY() < 0 || position.getY() > TheGame::Instance()->getWindowHeight()))
  {
    //find a path towards the center of the screen
    moveTowards(Vector2D(TheGame::Instance()->getWindowWidth()/2,
                         TheGame::Instance()->getWindowHeight()/2));
  }

  //else locate and move towards target
  else  
  {
    //if on frame to find new closest target
    if (framesToNextPathfind <= 0) {

      //locateNearestTarget will set new pathway should one exist
      nearestTarget = locateNearestTarget(); //get index of nearest target
      framesToNextPathfind = resetVal;  //reset number of frames countdown to find new path

      if (nearestTarget == -1)//no target was found
      { //go to random location
        pathFinder.findPath(position, generateRandomNearLocation()); //and generate a path to a random location
        framesToNextPathfind = resetVal*4;  //reset number of frames countdown to find new path to higher val
      }
    }

    //if a target available
    if (nearestTarget != -1) {
      //check if in contact with the currently tracked target,
      if (GameObject::checkForCollisionWithCircle(TheEnvironment::Instance()->targets[nearestTarget]))
      {
        //if so, run appropriate code to reduce size of target
        TheEnvironment::Instance()->targets[nearestTarget]->consumedByAgent();
        //and increase size and consumuption counter for agent
        consumedTarget();
      }
    }

    //if there is a path 
    if (pathFinder.pathway.size() > 0) {
      //continue to move towards the nearest target //USING Astar
      moveTowards(pathFinder.pathway.front()->getNodeCentralPosition());
    }
  }
    
  framesToNextPathfind--;
    
  GameObject::update();
}

void Agent::moveTowards(Vector2D target)
{
  //distance between agent and target
  Vector2D dirToMove = (target - position);

  //magnitude
  float hyp = dirToMove.magnitude();

  dirToMove.setX(dirToMove.getX() / hyp);
  dirToMove.setY(dirToMove.getY() / hyp);

  acceleration = dirToMove * (speedMultiplier / 100);
}

void Agent::clean()
{
  pathFinder.clean();
}

//calculates a path to the closest accessible targets and returns the index of it
int Agent::locateNearestTarget()
{
  //if there are currently targets within the environment
  if (TheEnvironment::Instance()->targets.size() > 0) {

    //init vec to hold distances between agent and all targets. index in vector is index of target
    std::vector<float> distances (TheEnvironment::Instance()->targets.size(), UINT_MAX);

    //for each target
    for (size_t i = 0; i < TheEnvironment::Instance()->targets.size(); i++)
    {
      //store the distance between the target and agent
      distances[i] = position.calcDistance(TheEnvironment::Instance()->targets[i]->position);
    }
      
    //set a limit on the number of target sources to check
    //(required as A* is run for each one if they are not close)
    unsigned numTargetsToCheck = 5;
    if (numTargetsToCheck > distances.size()) numTargetsToCheck = distances.size();

    //for the number of targets to check
    for (size_t i = 0; i < numTargetsToCheck; i++)
    {
      //find the closest one
      float minDistanceIndex = 0;
      for (size_t j = 1; j < distances.size(); j++)
      {        

        //if the distance is less
        if (distances[j] < distances[minDistanceIndex])
        {
          //save the index
          minDistanceIndex = j; 
        }   
      }
        
      //check if there is a path from the agent to the target
      if (distances[minDistanceIndex] != UINT_MAX)
      {
        if (pathFinder.findPath(position, TheEnvironment::Instance()->targets[minDistanceIndex]->position))
        {
          nearestTarget = minDistanceIndex;
          return nearestTarget; //if there is a path, return index of closest found target
        }
        else
        {
          distances[minDistanceIndex] = UINT_MAX; //else, remove from the list
        }
      }
    } //and loop and try the next closest target

    //if iterated numTargetsToCheck times and none were found, return no targets found
    nearestTarget = -1;
    return nearestTarget;
  }
  else //no targets found, return -1, no target found
  {
    nearestTarget = -1;
    return nearestTarget;
  }
}


void Agent::consumedTarget()
{

}

//randomly generate a location near to the agent's current position that is plus/minus 1/6 of the window size away for both width and height
Vector2D Agent::generateRandomNearLocation()
{
  //ints that will store rand gen coords
  int x, y;

  //amount of size to randomly add/minus from current position 
  int randSizeX = TheGame::Instance()->getWindowWidth() / 6;
  int randSizeY = TheGame::Instance()->getWindowHeight() / 6;


  do {
    x = position.getX() + (rand() % (randSizeX*2) - randSizeX);
  }
  while ( x >= TheGame::Instance()->getWindowWidth() || x < 0); //do until within the boundaries

  do {
    y = position.getY() + (rand() % (randSizeX*2) - randSizeX);
  }
  while ( y >= TheGame::Instance()->getWindowHeight() || y < 0);

  //return new random position
  return Vector2D(x,y);
}


