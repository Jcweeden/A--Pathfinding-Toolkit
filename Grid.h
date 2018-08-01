#ifndef _Grid_
#define _Grid_

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h" 

#include "Game.h"
#include "Vector2D.h"
#include "Node.h"
#include "GameObject.h"


#include <iostream>
#include <vector>

class Environment;

class Grid {

public:
  //the size of the grid
  Vector2D gridSize;

  //2D vector holding all nodes that form the grid
  std::vector<std::vector<Node*> > grid;
  
  float nodeRadius;
  //the width of a single node in px
  float nodeDiameter;

  //the count of how many nodes form the X and Y 
  unsigned nodeCountX, nodeCountY;

  //the coordinates of the selected agent to track the movement of
  Vector2D selectedObjectPosition;

  //holds the current set of Nodes that form the path of the selected Agent
  std::vector<Node*> pathway;

  //draw options (drawn on-screen if true)
  bool drawGridEnabled, drawTerrainEnabled, drawPathEnabled, drawPenCosts;

  //penalty costs for each terrain type
  unsigned groundTerrainPenCost, roadTerrainPenCost, mudTerrainPenCost;
  
public:

  //constructors
  Grid();
  Grid(unsigned p_gridLengthX, unsigned p_gridLengthY, float p_nodeDiameter, bool blurPenaltyCosts);

  //creates and put the new nodes in the nodes array. calls setObstacleNodesToUntraversable()
  void setupGrid(bool blurPenaltyCosts);

  //deletes nodes in grid
  void clean();

  //handles button presses to enable drawing of grid, terrain, path
  void handleInput();

  //resets gCost, fCost, heapIndex, and parent for each node
  void resetGrid();

  //sets all nodes to traverable
  void resetTraverable();
  
  //return the node at the coordinates passed in
  Node* getNodeFromCoords(Vector2D worldPosition);

  //check each obstacle against the grid, setting those nodes in contact with an obstacle to untraversable
  void setObstacleNodesToUntraversable();

  //check each terrain obj against the grid, setting nodes in contact with the appropriate penalty cost
  void setTerrainNodesPenaltyCost(std::vector<GameObject*> terrain, unsigned penaltyCost);

  //use a blur box to blur the penalty costs where differing terrain costs meet
  void blurTerrainPenaltyCosts();

  //reset all nodes to groundTerrainPenCost
  void resetTerrainPenCosts();

  //all traversable neighbours of the passed in node are returned, including those at diagonals
  std::vector<Node*> getNeighbouringNodes(Node* node);
  
  //for each, if enabled, draws and displays the grid, traversable terrain, and current path of selected agent
  void drawGrid();

  //draw just the path of the currently selected agent
  void drawPath();

  //draws terrain that can be traversed in green and untraversable in red
  void drawTraversableTerrain();

  //draw gradient heatmap on the terrain to show areas with higher penalty costs as a a darker colour
  void drawTerrainPenaltyCosts();
  
  //DEBUG - prints out representation of the grid, showing fCost for each node
  void printNodeValues(Node* startPos, Node* targetPos);

};

#endif
