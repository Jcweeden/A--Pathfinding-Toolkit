#ifndef _Node_
#define _Node_

#include "SDL2/SDL.h"
#include "Vector2D.h"

#include <vector>

class Node {

public:
  //the top left position of the node
  Vector2D position;

  //the index of the node in the Grid grid vector
  unsigned gridXIndex, gridYIndex;
  
  //the X width of the node
  unsigned gridWidth;
  //the Y height of the node
  unsigned gridHeight;
  
  //true if the node is not an obstactle, and is walkable
  bool traversable;

  //distance from starting node to this one
  int gCost;

  //distance from this node to target destination node
  int hCost;

  //additional cost added when terrain is harder to traverse
  //most usable terrain has cost of 1
  int terrainPenaltyCost;

  //coords of node's parent in the pathfinding algorithm. Used when retracing back through the search path
  Vector2D parent;

  //this node's position in the heap
  int heapIndex;

public:

  //constructors
  Node(Vector2D p_position, unsigned p_gridXIndex, unsigned p_gridYIndex, unsigned p_gridWidth, unsigned p_gridHeight, unsigned p_penaltyCost);

  Node();

  //the current system to hold nodes currently puts their position in the top left corner of the node
  //this method returns the coords of the node's central position
  Vector2D getNodeCentralPosition() { return Vector2D(position.getX() + gridWidth/2,
                                                      position.getY() + gridHeight/2);}
  
  //return the total cost for the node
  int fCost() {return gCost + hCost;}
};

#endif
