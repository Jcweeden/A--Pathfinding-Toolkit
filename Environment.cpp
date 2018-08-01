#include "Environment.h"

Environment* Environment::s_pInstance = 0;

Environment* Environment::Instance()
{
  if(s_pInstance == 0)
  {
    s_pInstance = new Environment();
    return s_pInstance;
  }

  return s_pInstance;
}

Environment::Environment()
{}

void Environment::setup(unsigned agentCount, unsigned targetCount, unsigned obstaclesCount, unsigned mudCount, unsigned roadsCount, unsigned gridSizeX, unsigned gridSizeY, unsigned nodeDiameter, unsigned setupDemoIndex, bool blurPenaltyCosts)
{

  dynamicObstacleDemo = false;
  selectedAgentIndex = 0;
  float defaultDampingVal = 0.6f;

  //init obstacles first so they nodes can be set to intraversble in the grid
  for (size_t i = 0; i < obstaclesCount; i++)
  {
    Obstacle *obstacle = new Obstacle();

    obstacle->setVelocity( 0,0);

    obstacle->setDamping(0.0f);
    obstacle->setAcceleration(0.0f, 0.0f); //no gravity

    obstacles.push_back(obstacle);
  }

   //init mud next so the nodes penalty costs can be set in the grid
  for (size_t i = 0; i < mudCount; i++)
  {
    Mud *mud = new Mud();

    mud->setVelocity( 0,0);

    mud->setDamping(0.0f);
    mud->setAcceleration(0.0f, 0.0f); //no gravity

    muds.push_back(mud);
  }

   //init road next so the nodes penalty costs can be set in the grid
  for (size_t i = 0; i < roadsCount; i++)
  {
    Road *road = new Road();

    road->setVelocity( 0,0);

    road->setDamping(0.0f);
    road->setAcceleration(0.0f, 0.0f); //no gravity

    roads.push_back(road);
  }
  
  std::cout << "Env.setup() before grid = Grid\n";
  grid = Grid(gridSizeX, gridSizeY, nodeDiameter, blurPenaltyCosts);

  
  //now init agents, ensuring they do not spawn on intraversable terrain
  for (size_t i = 0; i < agentCount; i++)
  { 
    Agent *agent = new Agent();
    
    //if and while the coords randomly selected put the agent into untraversable terrain
    //randomise position again, and loop until the object is placed in traversable terrain
    while (!(grid.getNodeFromCoords(agent->position)->traversable))
    {
      agent->setNewRandomisedPosition();  //generate a new location
    } 
    
    agent->setVelocity(0, 0);
    agent->setMass(1.0f);
    agent->setDamping(0.5f);
    agent->setAcceleration(0.0f, 0.0f); //no gravity

    //set the pathfinder to use the initialised grid
    agent->pathFinder.setGrid(&grid);
    
    agents.push_back(agent);
  }

  //init target sourcess, ensuring they do not spawn on intraversable terrain
  for (size_t i = 0; i < targetCount; i++)
  {
    Target *target = new Target();

    //if and while the coords randomly selected put the target into untraversable terrain
    //randomise position again, and loop until the object is placed in traversable terrain
    while (!(grid.getNodeFromCoords(target->position)->traversable))
    {
      target->setNewRandomisedPosition();  //generate a new location
    } 
    
    target->setVelocity(0,0);

    target->setDamping(0.0f);
    target->setAcceleration(0.0f, 0.0f); //no gravity
    target->clearAccumForces();

    targets.push_back(target);
  }

  if (setupDemoIndex != 99) setupDemo(setupDemoIndex);
  
  //init A* grid, runs setup and creates nodes in Grid constructor

  std::cout << "Env.setup() init grid = Grid\n";

}


void Environment::draw()
{  

  if (grid.drawPenCosts == false)
  {
    //draw each mud patch
    for (size_t i = 0; i < muds.size(); i++)
    {
      muds[i]->draw();
    }

    //draw each road on top of the mud
    for (size_t i = 0; i < roads.size(); i++)
    {
      roads[i]->draw();
    }
  
    //draw each obstacle on top of all this
    for (size_t i = 0; i < obstacles.size(); i++)
    {
      obstacles[i]->draw();
    }
  }
  
  //draw a black to white scale the penalty cost for each node
  grid.drawTerrainPenaltyCosts();

  //if enabled, draw the traversable terrain in green, and non-traversable in red
  grid.drawTraversableTerrain();
  
  //if enabled, draw the grid used for A* traversal 
  grid.drawGrid();

  //draw the path of selected agent
  grid.drawPath();
  
  //draw each target
  for (size_t i = 0; i < targets.size(); i++)
  {
    targets[i]->draw();
  }
  
  //draw each Agent on top of all this
  for (size_t i = 0; i < agents.size(); i++)
  {
    agents[i]->draw();
  }

}

void Environment::update()
{
  handleInput();
  grid.handleInput();
  
  if (dynamicObstacleDemo) {

    for (size_t i = 0; i < obstacles.size(); i++)
    {
      if (obstacles[i]->getPosition().getX() < 50)
        obstacles[i]->translateRight = true;
      else if (obstacles[i]->getPosition().getX() > 351)
        obstacles[i]->translateRight = false;

      if (obstacles[i]->translateRight)
        obstacles[i]->setPosition(Vector2D(obstacles[i]->getPosition().getX()+1, obstacles[i]->getPosition().getY()));
      else if (!obstacles[i]->translateRight)
        obstacles[i]->setPosition(Vector2D(obstacles[i]->getPosition().getX()-1, obstacles[i]->getPosition().getY()));

    }
    grid.resetTraverable();
    grid.setObstacleNodesToUntraversable();
  }
  
  for (size_t i = 0; i < targets.size(); i++)
  {
    //std::cout << "updating target " << i << "/" << targets.size()-1 <<"\n";
    targets[i]->update();
  }

  
  for (unsigned i = 0; i < agents.size(); i++)
  {    
    //std::cout << "updating agent " << i << "/" << agents.size()-1 <<"\n";
    agents[i]->update(); 
  }
  
  //if a agent is selected, update the grid to so that the selected agent's pathway can be drawn
  if (selectedAgentIndex != -1)
    grid.pathway = agents[selectedAgentIndex]->pathFinder.pathway;
  else //else clear the grid so that none is drawn
    grid.pathway.clear();    
}

void Environment::clean()
{
  for (Agent *agent: agents)
  {
    agent->clean();
    //delete from agents
    delete agent;
  }
    agents.clear();


  for (Target *target: targets)
  {
    //delete from targets
    delete target;
  }
    targets.clear();


  for (Obstacle *obs: obstacles)
  {
    //delete from obstacles
    delete obs;
  }
    obstacles.clear();
    

  for (GameObject *mud: muds)
  {
    //delete from mud
    delete mud;
  }
  muds.clear();

  for (GameObject *road: roads)
  {
    //delete from road
    delete road;
  }
    roads.clear();


  //delete all nodes in grid
  grid.clean();
}



void Environment::handleInput()
{
  //MOUSE
  if (TheInputHandler::Instance()->getMouseButtonState(0))
  {
    //get position of click
    Vector2D mousePos (TheInputHandler::Instance()->getMousePosition()->getX(),
                       TheInputHandler::Instance()->getMousePosition()->getY());

    //check with each agent if the click position was on that agent
    for (size_t i = 0; i < agents.size(); i++)
    {
      //if it was, then set that agent as selected, play the genotype, and return
      if (agents[i]->checkForCollisionWithPoint(&mousePos))
      {
        grid.drawPathEnabled = true;
        selectedAgentIndex = i;
        return;
      }
    }
    //else, no agent was selected - so select none
    //selectedAgentIndex = -1;

    //instead place the 0th target at this location

    targets[0]->setPosition(mousePos);

  }
}

void Environment::setupDemo(unsigned setupDemoIndex)
{
  switch (setupDemoIndex)
  {

    //1: selection of nearest target to collect
    case 1:
      //randomly place target and agent
      break;
    
      //2: path to goal with obstacle avoidance
    case 2:
      grid.drawTerrainEnabled = true;
      //randomly place obstacles and agent
      break;
      
      //3: dynamic obstacle avoidance
    case 3:
      dynamicObstacleDemo = true;
      
      grid.drawTerrainEnabled = true;
      //place obstacle and agent

      agents[0]->setPosition(Vector2D(300,50));
      agents[0]->speedMultiplier = 25;

      targets[0]->setPosition(Vector2D(300,520));
      
      obstacles[0]->setPosition(Vector2D(49,150));
      obstacles[0]->setWidth(200);
      obstacles[0]->setHeight(20);
      obstacles[1]->setPosition(Vector2D(351,250));
      obstacles[1]->setWidth(200);
      obstacles[1]->setHeight(20);
      obstacles[2]->setPosition(Vector2D(49,350));
      obstacles[2]->setWidth(200);
      obstacles[2]->setHeight(20);
      obstacles[3]->setPosition(Vector2D(351,450));
      obstacles[3]->setWidth(200);
      obstacles[3]->setHeight(20);
      obstacles[4]->setPosition(Vector2D(49,550));
      obstacles[4]->setWidth(200);
      obstacles[4]->setHeight(20);
      obstacles[5]->setPosition(Vector2D(351,50));
      obstacles[5]->setWidth(200);
      obstacles[5]->setHeight(20);

      grid.resetTraverable();
      grid.setObstacleNodesToUntraversable();
      break;

      //4: ability to determine if a viable route exists
    case 4:

      grid.drawTerrainEnabled = true;

      //place obstacles, target, agent
      agents[0]->setPosition(Vector2D(400,400));
      targets[0]->setPosition(Vector2D(240,240));
      
      obstacles[0]->setPosition(Vector2D(150,150));
      obstacles[0]->setWidth(10);
      obstacles[0]->setHeight(150);
      obstacles[1]->setPosition(Vector2D(300,150));
      obstacles[1]->setWidth(10);
      obstacles[1]->setHeight(150);
      obstacles[2]->setPosition(Vector2D(160,150));
      obstacles[2]->setWidth(150);
      obstacles[2]->setHeight(10);
      obstacles[3]->setPosition(Vector2D(160,290));
      obstacles[3]->setWidth(150);
      obstacles[3]->setHeight(10);
      

      grid.resetTraverable();
      grid.setObstacleNodesToUntraversable();
      break;

      //5/6: resizable A* navigation grid (size 20/30)
    case 5: case 6:
      grid.drawTerrainEnabled = true;
      grid.drawGridEnabled = true;

      agents[0]->setPosition(Vector2D(20,20));
      targets[0]->setPosition(Vector2D(400,440));
      
      //obs
      obstacles[0]->setPosition(Vector2D(530,195));
      obstacles[0]->setWidth(69);
      obstacles[0]->setHeight(71);
      
      obstacles[1]->setPosition(Vector2D(148,230));
      obstacles[1]->setWidth(58);
      obstacles[1]->setHeight(32);
      
      obstacles[2]->setPosition(Vector2D(541,547));
      obstacles[2]->setWidth(58);
      obstacles[2]->setHeight(32);
      
      obstacles[3]->setPosition(Vector2D(320,199));
      obstacles[3]->setWidth(90);
      obstacles[3]->setHeight(88);

      obstacles[4]->setPosition(Vector2D(333,121));
      obstacles[4]->setWidth(34);
      obstacles[4]->setHeight(75);
      
      obstacles[5]->setPosition(Vector2D(186,298));
      obstacles[5]->setWidth(81);
      obstacles[5]->setHeight(92);
      
      obstacles[6]->setPosition(Vector2D(394,305));
      obstacles[6]->setWidth(20);
      obstacles[6]->setHeight(81);
      
      obstacles[7]->setPosition(Vector2D(134,67));
      obstacles[7]->setWidth(107);
      obstacles[7]->setHeight(97);

      obstacles[8]->setPosition(Vector2D(100,386));
      obstacles[8]->setWidth(39);
      obstacles[8]->setHeight(83);
      
      obstacles[9]->setPosition(Vector2D(68,234));
      obstacles[9]->setWidth(18);
      obstacles[9]->setHeight(36);

      obstacles[10]->setPosition(Vector2D(103,400));
      obstacles[10]->setWidth(130);
      obstacles[10]->setHeight(89);
      
      grid.resetTraverable();
      grid.setObstacleNodesToUntraversable();
      break;

    case 7: case 8:
      grid.drawPenCosts = true;

      //set agent and target position
      agents[0]->setPosition(Vector2D(30,30));
      targets[0]->setPosition(Vector2D(400,550));
      
      //set roads
      roads[0]->setPosition(Vector2D(0,260));
      roads[0]->setWidth(600);
      roads[0]->setHeight(99);

      roads[1]->setPosition(Vector2D(260,0));
      roads[1]->setWidth(99);
      roads[1]->setHeight(600);

      //set mud
      muds[0]->setPosition(Vector2D(100,100));
      muds[0]->setWidth(400);
      muds[0]->setHeight(400);

      
      //reset penalty costs
      grid.resetTerrainPenCosts();

      //set new penalty costs
      grid.setTerrainNodesPenaltyCost(TheEnvironment::Instance()->muds, 20);
      grid.setTerrainNodesPenaltyCost(TheEnvironment::Instance()->roads, 1);

      //if demo where blurred, then call to blur
      if (setupDemoIndex == 8) grid.blurTerrainPenaltyCosts();

      //reset obstacles
      grid.resetTraverable();
      grid.setObstacleNodesToUntraversable();
      
      break;
  }
}

