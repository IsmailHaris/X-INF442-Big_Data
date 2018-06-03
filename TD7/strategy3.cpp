#include "matrix.h"
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

unsigned int *Matrix::vectorProduct(unsigned int *b) {
  // assumes the argument b has the correct length n.
  // creates a new unsigned int *, which must be deleted by the user.

  const int root = 0;

  int rank, size;

  unsigned int *c = new unsigned int[n];
  for (unsigned int i = 0; i < n; ++i) {
    c[i] = 0;
  }

  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // TODO Exercise 3:
  // Matrix-vector product c = Ab with parallel decomposition by blocks.
  // Each of the p processors handles the matrix entries in a contiguous rectangle of side-lengths floor(n/p) or ceil(n/p).
  // The vectors b and c are both decomposed into blocks for each processor.

  long double sqrt_p = sqrt(size);

  if ((sqrt_p - floor(sqrt_p)) == 0){

    unsigned int sqrt_size = int(sqrt_p);
    unsigned int q=n/sqrt_size;
    unsigned int r=n%int(sqrt_size);
    unsigned int size_i;
    unsigned int size_j;
    unsigned int start_i;
    unsigned int start_j;

    // Disjonctoin de cas en fonction de n, p (size) et r=n%p
    if (n<=sqrt_size){ // S il y a plus de processeurs par ligne/col que de n
      if (rank<pow(n,2)){
          start_i = rank/n;
          start_j = rank%n;
          size_i = 1;
          size_j = 1;
      }else{
          size_i = 0;
          size_j = 0;
      }
    }else{
    	unsigned int block_i = rank/sqrt_size;
    	unsigned int block_j = rank%sqrt_size;
    	if(block_i < r && block_j < r){ // En haut à gauche
          start_i = block_i*(q+1);
          start_j = block_j*(q+1);
          size_i = q+1;
          size_j = q+1;
    	}else if(block_i < r && block_j >= r){ // En haut à droite
    	  start_i = block_i*(q+1);
          start_j = r*(q+1)+(block_j-r)*q;
          size_i = q+1;
          size_j = q;
		}else if(block_i >= r && block_j < r){ // En bas à gauche
    	  start_i = r*(q+1)+(block_i-r)*q;
    	  start_j = block_j*(q+1);
          size_i = q;
          size_j = q+1;
    	}else if(block_i >= r && block_j >= r){ // En bas à droite
    	  start_i = r*(q+1)+(block_i-r)*q;
    	  start_j = r*(q+1)+(block_j-r)*q;
          size_i = q;
          size_j = q;
    	}
    }


    // Calcul du résultat local de chaque processeurs
    unsigned int *c_local = new unsigned int[n];
    for (unsigned int i = 0; i < n; ++i) {
      c_local[i] = 0;
    }

    for (int i=start_i; i<start_i+size_i; i++){
      unsigned int s = 0;
      for (int j=start_j; j<start_j+size_j; j++){
          s += (data[i][j] * b[j]);
      }
      c_local[i] = s%26;
    }

    if(rank != root){ // Les processeurs esclaves envoient au root
      MPI_Send(c_local, n, MPI_UNSIGNED, root, 0, MPI_COMM_WORLD);
    }else{ // Le processeur root procède à la réduction %26;
      c = c_local;
      for (int p=1; p<size; p++){
        unsigned int *c_recv = new unsigned int[n];
        MPI_Recv(c_recv, n, MPI_UNSIGNED, p, 0, MPI_COMM_WORLD, &status);
        for(int j=0; j<n; j++){
          c[j] += c_recv[j];
        }
      }
      for(int j=0; j<n; j++){
        c[j] = c[j]%26;  // don't forget to reduce the result modulo 26!
      }
    }

  }else{
    if (rank == root){
      std::cout << "Le nombre de processeurs p doit être un carré parfait ! Ici : " << size << std::endl;
    }
    // else die silently
  }

  return c;
}
