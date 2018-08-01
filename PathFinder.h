#ifndef _PathFinder_
#define _PathFinder_

#include "SDL2/SDL.h"

#include "Heap.h"
#include "Grid.h"
#include "Vector2D.h"
#include "Node.h"


#include <iostream>
#include <vector>

class PathFinder {

public:

  //pointer to the grid used in pathfinding
  Grid* grid;

  //holds the current set of Nodes that forms the path of the selected Agent
  std::vector<Node*> pathway;

  //min heap to hold the grid nodes used in pathfinding
  Heap heap;

public:

  //constructor
  PathFinder();

  //setter - sets grid that will be used in findPath()
  void setGrid(Grid* p_grid);
  
  //undertakes A* pathfinding to traverse the grid from startPosition to targetPosition
  //return true if a path was found
  bool findPath(Vector2D startPosition, Vector2D targetPosition);

  //calculate a distance heuristic (movement cost) between two nodes in the grid
  int calcMovementCost(Node* nodeA, Node* nodeB);

  //starting from the target node a vector has added the end node, and then parent of each node,
  //retracing the shortest path back to the start position. 
  void retracePath(Node* start, Node* end);

  //simply clears the pathway vector
  void clean();
};

#endif
