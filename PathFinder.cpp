#include "PathFinder.h"
#include <list>
PathFinder::PathFinder()
    : grid(new Grid())
{

}

void PathFinder::setGrid(Grid* p_grid)
{
  grid = p_grid;
}

bool PathFinder::findPath(Vector2D startingPosition, Vector2D targetPosition)
{
  //using start and end coords get the start and target nodes in the grid
  Node* startPositionNode = grid->getNodeFromCoords(startingPosition);
  Node* targetPositionNode = grid->getNodeFromCoords(targetPosition);
  
  //if the start position is null (e.g., getNodeFromCoords returns (-1,-1) for coords that are not
  //on the map), then return as there will be no viable path
  if (!startPositionNode || !targetPositionNode)
  { //then do not find a new path, return
    return false;
  }
  //or if the target node is not traversable then return also
  else if (targetPositionNode->traversable == false)
  {
    return false;
  }

  //set size of heap to that of the grid (to max amount of nodes that could be evaluated)
  heap.setHeapSize(grid->nodeCountX*grid->nodeCountY);

  //closedSet will hold all evaluated nodes that have had all neighbours checked
  std::vector<Node*> closedSet; 

  //add first node to end of list of openedNodes
  heap.addItem(startPositionNode);

  //while there are still nodes to check
  while (heap.getHeapSize() > 0)
  {
    //GET NODE IN OPEN SET WITH LOWEST fCost
    Node* node = heap.removeFirstItem();  //retrieve the first node (lowest fCost) for comparision
    closedSet.push_back(node); //node will be evaulated so can be added to closedSet already
    
    //IF IS THE TARGET NODE
    if (node == targetPositionNode)
    {
      //RETRACE
      retracePath(startPositionNode, targetPositionNode);
      // std::cout << "heap.size() upon completion of path: " << heap.getHeapSize() << "\n";

      //run cleanup
      closedSet.clear();
      heap.clean();
      //grid->resetGrid(); //does not affect running of simulation, but required for memory deallocation

      return true; //return true a path was found
    }
    
    //FOR EACH NEIGHTBOURING NODE OF CURRENT NODE
    for (Node* neighbour : grid->getNeighbouringNodes(node))
    {
      //if this neighbour is intraversable,
      //or is in closedSet (has been evaluated already), then ignore this neighbour
      if (neighbour->traversable == false ||
          std::find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end()) continue;

      //otherwise, check IF a NEW PATH TO this NEIGHBOUR IS SHORTER THAN it's OLD PATH (or this may be the first path found to this node)
      //(a neighbour may have already had its gCost calculated, but with exploration of new nodes
      //it is possible that a shorter path has been found so we must update that node's cost)
      
      //calculate movement cost to neighbour (cost to current node + cost from that node to this one + terrain cost for the node)
      int newMovementCostToNeighbour = node->gCost + calcMovementCost(node, neighbour) + neighbour->terrainPenaltyCost;

      //if the path from the currently being checked node to this neighbour is lower, or if this
      //neighbour node has never been checked before
      if (newMovementCostToNeighbour < neighbour->gCost || !heap.isInHeap(neighbour) )
      {
        //then update neighbour's costs and set it's parent to the currently checked node,
        //as this is the shortest path to this node that has currently been found
        neighbour->gCost = newMovementCostToNeighbour;
        neighbour->hCost = calcMovementCost(neighbour,targetPositionNode);
        neighbour->parent = Vector2D(node->gridXIndex,node->gridYIndex);

        //if the neighbour being checked is not already in the heap
        if (!heap.isInHeap(neighbour))
        { //then add it to the heap
          heap.addItem(neighbour);
        }
        else //otherwise, as it has had it's fCost updated
        { //sort the heap to put the updated neighbour in the correct position
          heap.sortUp(neighbour);
        }
      }
    }
  }
  //return to alleviate warning - return false if all paths explored and no route to target found.
  return false;
}

void PathFinder::retracePath(Node* start, Node* end)
{
  pathway.clear(); //discard the old path
  
  Node* currentNode = end; //get the target end node

  //starting from the target node loop backwards through the path storing the node
  //and then its parent until reaching the start node, where we will have stored
  //the complete found path
  while (currentNode != start) 
  {
    //as retracing path backwards insert the current node at the front of the path
    pathway.insert(pathway.begin(), currentNode);
    
    //and then get its parent
    currentNode = grid->grid[currentNode->parent.getX()][currentNode->parent.getY()];
  }
}

int PathFinder::calcMovementCost(Node* nodeA, Node* nodeB)
{
  //get x and y distance between nodeA and nodeB  
  int distanceX = abs((int)nodeA->gridXIndex - (int)nodeB->gridXIndex);

  int distanceY = abs((int)nodeA->gridYIndex - (int)nodeB->gridYIndex);

  //to calculate the distance we use formula:
    //if (x > y) apply: 14y + 10(x - y)
    //if (y > x) apply: 14x + 10(y - x)  
  
  //from X & Y: substract lower from higher number to determine num of horizontal moves across to target (multiply by 10 to get total distance for horiz move)
  //plus
  //vert moves multi by 14 (is cost for diagonal movement)
  if (distanceX > distanceY) return (14 * distanceY) + (10 * (distanceX-distanceY));

  else return (14 * distanceX) + (10 * (distanceY-distanceX));
}

void PathFinder::clean()
{
  pathway.clear();
  //grid->resetGrid();
}
