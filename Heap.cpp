#include "Heap.h"

Heap::Heap()
{
  itemsCount = 0;
}

void Heap::setHeapSize(int count)
{
  items.resize(count); //allocate space for the size of the grid
}

//resets the costs and heap index's for all nodes used in a pathfinder search
//after running the heap will be ready for next pathfinding run.
void Heap::clean()
{
  
  for(size_t i = 0; i <= itemsCount; i++)
  {
    items[i]->hCost = 0;
    items[i]->gCost = 0;
    items[i]->heapIndex = 0;
  }
  
  items.clear();
  itemsCount = 0;
}

void Heap::addItem(Node* item)
{
  //set the item's heapIndex to reflect it's position
  item->heapIndex = itemsCount;
  //add to the heap
  items[itemsCount] = item;
  //run sortUp to order it into correct position
  sortUp(item);

  itemsCount++;
}

//remove first item from the heap and set final item in heap to its position
Node* Heap::removeFirstItem()
{
  Node* firstItem = items[0];

  itemsCount--;

  //take item at end of heap and put into first place
  items[0] = items[itemsCount];
  items[0]->heapIndex = 0;

  //then sortDown to rearrange new placed node into correct position
  sortDown(items[0]);

  //return the removed item
  return firstItem;
}

//swap the locations in the heap of two nodes
void Heap::swapItems(Node* itemA, Node* itemB)
{
  //swap locations in array
  items[itemA->heapIndex] = itemB;
  items[itemB->heapIndex] = itemA;

  //swap heapIndexs of items
  int tempIndex = itemA->heapIndex;

  itemA->heapIndex = itemB->heapIndex;
  itemB->heapIndex = tempIndex;

}

//check if node is in heap
bool Heap::isInHeap(Node* item)
{
  //check if item's heap index exists in the array
  if (items[item->heapIndex] == item)
      return true;
    else
      return false;
}

void Heap::sortUp(Node* item)
{
  //get parent index of node item
  int parentIndex = (item->heapIndex-1)/2;

  //true while item's fCost is lower than the item's parent
  while (true)
  {
    Node* parentItem = items[parentIndex];

    //if item has lower fCost than its parent
    if (item->fCost() < parentItem->fCost())
    {
      //swap the item's position in the heap with its parent
      swapItems(item, parentItem);
    }
    else
    {
      break; //no longer a higher priority than its parent, in correct position in heap
    }
      
    parentIndex = (item->heapIndex-1)/2;
  }
}

void Heap::sortDown(Node* item)
{
  while(true)
  {
    //get the heap index of parent item's two children
    int childIndexLeft = item->heapIndex * 2 + 1;
    int childIndexRight = item->heapIndex * 2 + 2;

    //temp variable to hold index of child with lowest fCost
    int lowestfCostChildIndex = 0;

    //if item has at least one child
    if (childIndexLeft < itemsCount)
    {
      //save child index left as current lowest fCost child
      lowestfCostChildIndex = childIndexLeft;

      //if item has two children (child on right)
      if (childIndexRight < itemsCount)
      {
        //if child on right has lower fCost than the saved index child on left
        if (items[childIndexRight]->fCost() < items[childIndexLeft]->fCost())
        { //then get the index as the right child has lower fCost
          lowestfCostChildIndex = childIndexRight;
        }
      }
      //lowestfCostChildIndex is now equal to child with lowest fCost
      
      //check if parent has higher fCost than child with lowest fCost
      if (item->fCost() > items[lowestfCostChildIndex]->fCost()) 
      {
        //if so, then swap them
        swapItems(item, items[lowestfCostChildIndex]);
      }
      
      //else parent has higher priority, exit loop as heap is in order
      else return;   
    }
    //parent has no children, so is in correct position
    else return;
  }
}



//DEBUG - for printing out tree


void Heap::print()
{
  if (itemsCount > 0)
    print_tree(items);
  else
    std::cout << "Heap.print():: no items in tree to print";

    std::cout << "\n";
}

std::string do_padding (unsigned index, unsigned mlength){
  std::string padding;
  if (int((index-1)/2) != 0){
    return (int((index-1)/2) % 2 == 0) ?
    (do_padding(int((index-1)/2),mlength) + std::string(mlength+4,' ') + " ")  :
    (do_padding(int((index-1)/2),mlength) + std::string(mlength+3,' ') + " |") ;
  }
  return padding;
}

void Heap::printer (std::vector<Node*> const & tree, unsigned index, unsigned mlength){
  auto last = itemsCount - 1 ;
  auto  left = 2 * index + 1 ;
  auto  right = 2 * index + 2 ;
  std::cout << " " << tree[index]->fCost() << " ";
  if (left <= last){
    auto llength = std::to_string(tree[left]->fCost()).size();
    std::cout << "---" << std::string(mlength - llength,'-');
    printer(tree,left,mlength);
    if (right <= last) {
      auto rlength = std::to_string(tree[right]->fCost()).size();
      std::cout << "\n" << do_padding(right,mlength) << std::string(mlength+ 3,' ') << " | ";
      std::cout << "\n" << do_padding(right,mlength) << std::string(mlength+ 3,' ') << " └" <<
      std::string(mlength - rlength,'-');
      printer(tree,right,mlength);
    }
  }
}

void Heap::print_tree (std::vector<Node*> & tree){
  unsigned mlength = 0;
  for (size_t i = 0; i < itemsCount; i++){
    auto clength = std::to_string(tree[i]->fCost()).size();
    if (clength > mlength) {
      mlength = std::to_string(tree[i]->fCost()).size();
    }
  }
  std::cout <<  std::string(mlength- std::to_string(items[0]->fCost()).size(),' ');
  printer(tree,0,mlength);
}


