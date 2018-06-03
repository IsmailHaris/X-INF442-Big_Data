#include "matrix.h"
#include <mpi.h>
#include <math.h>

unsigned int *Matrix::vectorProduct(unsigned int *b) {
  // assumes the argument b has the correct length n.
  // creates a new unsigned int *, which must be deleted by the user.

  const int root = 0;

  unsigned int *c = new unsigned int[n];
  for (unsigned int i = 0; i < n; ++i) {
    c[i] = 0;
  }

  int rank, size;

  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // TODO Exercise 1: 
  // Matrix-vector product c = Ab with parallel decomposition by rows.
  // Each of the p processors computes with floor(n/p) or ceil(n/p) rows of this matrix.
  // The vectors b and c are replicated on each processor.

  // Chaque processeur calcule la taille des données qu'il va utiliser dans ses calculs

  unsigned int q=n/size;
  unsigned int r=n%size;
  unsigned int size_local;
  unsigned int start;

  // Disjonctoin de cas en fonction de n, p (size) et r=n%p
  if (n<=size){
    if (rank<n){
        start = rank;
        size_local = 1;
    }else{
        size_local = 0;
    }
  }else{
    if(rank<r){
        size_local = q+1;
        start = rank*(q+1);
    }else{
        size_local = q;
        start = r*(q+1)+(rank-r)*q;
    }
  }


  // Calcul du résultat local de chaque processeurs
  unsigned int *c_local = new unsigned int[n];
  for (unsigned int i = 0; i < n; ++i) {
    c_local[i] = 0;
  }

  for (int i=start; i<start+size_local; i++){
    unsigned int s = 0;
    for (int j=0; j<n; j++){
        s += (data[i][j] * b[j]);
    }
    c_local[i] = s % 26;
  }

  /* Debug
  if(n==2){
    for (int i=0;i<n;i++){
        std::cout << "# Proc : " << rank << " || c_local = " << c_local[i] << " ";
    }
    std::cout << std::endl;
  }
  */

  if(rank != root){ // Les processeurs esclaves envoient au root
    MPI_Send(c_local, n, MPI_UNSIGNED, root, 0, MPI_COMM_WORLD);
  }else{ // Le processeur root procède à la réduction %26;
    c = c_local;
    for (int p=1; p<size; p++){
      unsigned int *c_recv = new unsigned int[n];
      MPI_Recv(c_recv, n, MPI_UNSIGNED, p, 0, MPI_COMM_WORLD, &status);
      for(int j=size_local-1; j<n; j++){
        c[j] += c_recv[j]%26;  // don't forget to reduce the result modulo 26!
      }
    }
  }

  // delete[] c_local;

  return c;
}
