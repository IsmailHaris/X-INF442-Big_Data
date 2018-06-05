#include <vector>

#include "mpi.h"
#include "computeNeighbors.hpp"

void broadcast() {
  const int root = 0;
  int myrank, size;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int round = 0;
  int tag = 0;

  // root does not have to wait for a message
  if (myrank != root) {
    // // TODO : wait for message from predecessor and print out the round
    int pred = computePredecessor(myrank); 
    MPI_Recv(&round, 1, MPI_INT, pred, 0, MPI_COMM_WORLD, &status);
    std::cout << "node " << myrank << " received message from " << pred << " in round " << round << std::endl;
  }

  // now let's send

  // TODO : first compute neighbors to send to
  std::vector<int> neighbors = computeOutNeighbors(myrank, size);

  // TODO : iterate over neighbors and send
  for(std::vector<int>::iterator it=neighbors.begin(); it!=neighbors.end(); it++){
    round++;
    MPI_Send(&round, 1, MPI_INT, *it, 0, MPI_COMM_WORLD);
  }

}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  broadcast();
  MPI_Finalize();
  return 0;
}
