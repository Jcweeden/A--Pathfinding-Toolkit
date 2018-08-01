#ifndef _Agent_
#define _Agent_

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h" 

//#include "GameObject.h"
#include "Environment.h"
#include "PathFinder.h"

#include <iostream>
#include <string>

class GameObject;

class Agent : public GameObject {
public:

  //the unique number of the agent within the simulation (for UI purposes only)
  unsigned agentID;
  
  //the radius of the agent upon spawning
  float startingRadius;
  
  //starts at 10 and is decremented as the agent increases in size and gets slower
  float speedMultiplier;

  //the index of the target that the agent is currently heading towards
  int nearestTarget;

  //holds the grid that the agent uses to navigate the environment, and produces a path given the mircobe's
  //location and a target location (e.g. target or mate)
  PathFinder pathFinder;
  
  //the pathfinding algorithm only runs every x amount of frames. This is the number of frames to elapse before determining a new path
  int framesToNextPathfind;


public:

  Agent(); //constructor

  //draws the agent and it's child (should it have eaten enough target)
  void draw();

  //determines the action of the agent, depending on how much target it has eaten, and whether it is dying
  //calls appropriate functions to get a path to the nearest target/partner as required, and applies forces
  //so that the agent will move on this path
  void update();

  //cleans the class 
  void clean();

  //given a target location the agent has the appropriates forces applied to propell it in that direction
  //generally passed the first node in the path found by pathfinder's A* route
  void moveTowards(const Vector2D target);

  /**
   //calculates a path to the closest accessible target and returns the index of it
   1. calculates the distance between the agent and each target
   2. checks the closest target to see if there is a traversable path, if so stores the index of the closest target and returns it,
   3. else iterates through targets until a viable path is found to the closest target to the agent. 
  **/
  int locateNearestTarget();

  
  //randomly generate a location near to the agent's current position that is plus/minus 1/6 of the window size away for both width and height
  Vector2D generateRandomNearLocation();

  //increments foodEaten, reduces the agent's speed, adds width to agent and the child being drawn within it
  void consumedTarget();

};

#endif
