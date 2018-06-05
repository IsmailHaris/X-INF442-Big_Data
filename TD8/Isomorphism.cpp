#include <iostream>
#include <stdexcept>
#include <map>
#include "Isomorphism.hpp"
#include "mpi.h"
using namespace std;

// Exercice 3
void extendInjection(Mapping &m) {
  // The argument m is (a reference to) a partially-defined injective
  // function G -> H : it may only have images defined for a subset of G.
  // This procedure recursively attempts to extend m to the whole of G,
  // using a backtracking approach.

  if (m.isFull()) {
    return;
  }
  for (int nidg = 0; nidg < m.getSizeG(); nidg++) {
    for (int nidh = 0; nidh < m.getSizeH(); nidh++) {
      if(m.areMappable(nidg,nidh)){
        m.addToMap(nidg,nidh);
        extendInjection(m);
        if (m.isFull()) return;
        else m.deleteFromMap(nidg);
      }
    }
  }
  return;
}

void findIsomorphism(Graph G, Graph H) {
  if (G.vertexCount() != H.vertexCount()) {
    std::cout << "Pas d'isomorphisme" << std::endl;
    return;
  }
  Mapping candidate(G, H);
  extendInjection(candidate);
  if (candidate.isFull())
    std::cout << candidate << std::endl;
  else
    std::cout << "Pas d'isomorphisme" << std::endl;
}

// Exercice 4
void findSubIsomorphism(Graph G, Graph H) {
  Mapping candidate(G, H);
  extendInjection(candidate);
  if (candidate.isFull())
    std::cout << candidate << std::endl;
  // ...si aucun sous-isomorphisme trouve,
  else
  std::cout << "Pas de sous-isomorphisme" << std::endl;
}

// Exercice 5
void findSubIsomorphismMPI(Graph G, Graph H) {
  int myrank, size;
  int n = H.vertexCount();

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;

  std::vector<Mapping> results;

  if (myrank == 0) {
    for (int nidg = 0; nidg < G.vertexCount(); nidg++) {
      for (int nidh = 0; nidh < H.vertexCount(); nidh++){
        Mapping candidate(G, H);
        if(candidate.areMappable(nidg,nidh)){
          candidate.addToMap(nidg,nidh);
          extendInjection(candidate);
          if (candidate.isFull()) results.push_back(candidate);
        }
      } 
    }

    for (std::vector<Mapping>::iterator it = results.begin(); it != results.end(); ++it){
        std::cout << *it <<std::endl; // Note the "*" here
    }
  } else {
    // TODO : Completer ici
  }
  return;
}
