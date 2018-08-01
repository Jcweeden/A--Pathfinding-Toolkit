#ifndef _Environment_
#define _Environment_

#include "SDL2/SDL.h"
#include "Vector2D.h"

#include "GameObject.h"
#include "Agent.h"
#include "Target.h"
#include "Obstacle.h"
#include "Road.h"
#include "Mud.h"
#include "Grid.h"
#include "PathFinder.h"

#include <iostream>
#include <string>
#include <vector>

class Agent;
class Target;
class Obstacle;
class Mud;
class Road;

class Environment {

public:
  
  //static instance variable
  static Environment* s_pInstance;

  //vector containing all active target sources
  std::vector<Target*> targets;
  
  //vector containing all active agents
  std::vector<Agent*> agents;

  //vector containing all obstacles in the environment
  std::vector<Obstacle*> obstacles;

  //if enabled the obstacle will move around the terrain
  bool dynamicObstacleDemo;

  //terrain that has the lowest penalty cost
  std::vector<GameObject*> roads;

  //terrain that has the highest penalty cost
  std::vector<GameObject*> muds;
  
  //grid that holds the terrain for A* traversal of agents
  Grid grid;
  
  //the index of the selected agent whose path is drawn
  int selectedAgentIndex;
  
public:
  //create singleton instance
  static Environment* Instance();

  Environment();

  //instantiates agents, targets and obstacles within the simulation and creates the grid that is used
  //for A* path traversal
  void setup(unsigned agentCount, unsigned targetCount, unsigned obstaclesCount, unsigned mudCount, unsigned roadsCount, unsigned gridSizeX, unsigned gridSizeY, unsigned nodeDiameter, unsigned setupDemoIndex, bool blurPenaltyCosts);

  //places all objs in environment in same place for each demo, rather than randomising positions
  void setupDemo(unsigned setupDemoIndex);
  
  void draw();

  void update();

  void clean();

  //check the position of mouse clicks, and if a agent is clicked then set it as selected
  void handleInput();
};

typedef Environment TheEnvironment;


#endif
