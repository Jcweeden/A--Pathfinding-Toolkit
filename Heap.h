#ifndef _Heap_
#define _Heap_

#include "Node.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


class Heap {

public:
//the array that contains and is ordered to hold all nodes in the min heap
std::vector<Node*> items;

//the number of items in the items vector (items.size() is not viable in this use case as the size of the items vector is reserved before use)
int itemsCount;

public:

//constructor
Heap();

//adds the item to the end of the heap, and then runs sortUp to reorder the heap and place the item correctly
void addItem(Node* item);

//removes the first item from the heap and sets the final item in heap to its position
//then runs sortDown() to rearrange this item and the heap into correct order.
//as a min heap first item removed will have the lowest fCost of all those in the heap
Node* removeFirstItem();

//swap the locations in the heap of two nodes (used in sortUp & sortDown)
void swapItems(Node* itemA, Node* itemB);

//the heap vector is allocated a size of the grid upon beginning findPath()
void setHeapSize(int count);

//returns the number of items currently in the heap
int getHeapSize() {return itemsCount; }

//using the item's heapIndex, it checks it against this position in items to check if the item is in the heap
bool isInHeap(Node* item);

//when an item is added to the end of the tree its priority must be taken into account and the heap reordered.
//a loop is run in which the item's fCost is compared with that of it's parent, and swapped if it has a higher
//priority
void sortUp(Node* item);

//runs opposite to sortUp(). Run when item is placed at front of heap. Item's fCost is compared with it's two
//children and swapped until it is no longer of a higher fCost than it's children and therefore in the correct
//position in the heap
void sortDown(Node* item);

//after running clean() the heap will be ready for next pathfinding run
void clean();

//three methods used for debugging to print out heap
void print();
void printer (std::vector<Node*> const & tree, unsigned index, unsigned mlength);
void print_tree (std::vector<Node*> & tree);
};

#endif

