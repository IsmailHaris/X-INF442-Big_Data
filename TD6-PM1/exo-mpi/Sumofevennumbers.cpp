#include "mpi.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>

// In this exercise, you have to fill the sections *** TO COMPLETE ***

int main(int argc, char *argv[]) {
    const int root = 0;

	// Initialize MPI
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int nprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	// Read parameter N as first argument of the call
    if (argc != 2) {
      if (rank == root) {
        std::cerr << "Usage : " << argv[0]
                  << " N (where N = # of even numbers to consider)"
                  << std::endl;
      } else {
        // print nothing, fail quietly
      }
      MPI_Finalize();
      return 1;
    }

    int N;

    if(rank == 0){
    	N = atol(argv[1]);
    }

	// TODO: the root decides how may numbers will be handled by each of the
	// processors, and MPI_Scatters its decision
	// *** TO COMPLETE ***

	MPI_Bcast(&N, 1, MPI_INT, root, MPI_COMM_WORLD);

    int q = N/nprocs;
    int r = N%nprocs;
    int size_local;

    // On réparti le reste sur les r premiers processeurs
    if(rank < r){
        size_local = q+1;
    }else{
        size_local = q;
    }

    long sum_local = 0;

    for (int i=q*rank; i<q*rank+size_local; i++){
        sum_local = sum_local+2*i;
    }

	// Local computation	
	long errors;
	// *** TO COMPLETE ***
	
	// Global computation with MPI
	long total;
	// *** TO COMPLETE ***
	MPI_Reduce(&sum_local, &total, 1, MPI_LONG, MPI_SUM, root, MPI_COMM_WORLD); 

	// Compute global sum and output result
	// To check it is correct, you can use the known fomula ****
	if (rank == 0) {
		std::cout << total << std::endl;
	}
	
	// Finalize and exit
	MPI_Finalize();
}
