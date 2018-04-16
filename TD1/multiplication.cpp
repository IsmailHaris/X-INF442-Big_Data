#include <iostream>
 
int** lireMatrice(int& n, int& m){
    // lecture de la taille de la matrice
    
    n = 0;
    m = 0;
    
    std::cin >> n;
    std::cin >> m;
    
    // 1. creation de la matrice en memoire et lecture de la matrice
    // on doit creer un tableau 1D de pointeurs de type int*
    int** matrice = new int*[n];
    
    // on cree maintenant les lignes
    for (int i=0; i<n; i++){
        matrice[i] = new int[m];
    }
    
    // on lit la matrice
    for (int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            std::cin >> matrice[i][j];
        }
    }
    
    return matrice;
}
 
int main(int argc, char** argv){
  int nA, mA;
  int** A = lireMatrice(nA, mA);
 
  int nB, mB;
  int** B = lireMatrice(nB, mB);
 
  int** C = new int*[nA];

  for (int i=0; i<nA; i++){
    C[i] = new int[mB];
  }

    // on lit la matrice
  for (int i=0; i<nA; i++){
    for (int j=0; j<mB; j++){
      for (int k=0; k<mA; k++) C[i][j] = C[i][j]+A[i][k]*B[k][j];
    }
  }

  for (int i=0; i<nA; i++){
    for (int j=0; j<mB; j++){
      std::cout << C[i][j];
      if (j < mB-1) std::cout << " ";
    }
    std::cout << std::endl;
  }
 
  return 0;
}