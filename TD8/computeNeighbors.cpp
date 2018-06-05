#include <iostream>
#include <vector>
#include <math.h>

// messages are received from node with highest 1-bit flipped to 0
int computePredecessor(int node) {
  // TODO
  if (node==0)
    return 0;
  
  int i=0;
  while(pow(2,i)<=node){
    i++;
  }
  return node-pow(2,i-1);
}

// compute neighbors to communicate to
std::vector<int> computeOutNeighbors(int node, int numberNodes) {
  std::vector<int> neighbors;

  // first bit to flip
  int flipbit = (node - computePredecessor(node)) * 2;

  // TODO
  int next = 0;
  if (node==0){
    next = 1;
    while(next<numberNodes){
      if (next<numberNodes){
        neighbors.push_back(next);
      }
      next = next*2;
    }
  }else{
    while(next<numberNodes){
      next = node + flipbit;
      if (next<numberNodes){
        neighbors.push_back(next);
        flipbit = flipbit*2;
      }
    }
  }

  return neighbors;
}

// print neighbor list for debugging
void printNeighbors(std::vector<int> neighbors) {
  for (std::vector<int>::iterator iter = neighbors.begin();
       iter != neighbors.end(); iter++) {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;
}
