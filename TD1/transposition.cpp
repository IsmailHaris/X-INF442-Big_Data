#include <iostream>

int main(int argc, char** argv){
    // lecture de la taille de la matrice
    
    int n = 0;
    int m = 0;
    
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
    
    // 2. affichage de la matrice transposee
    for (int j=0; j<m; j++){
        for (int i=0; i<n; i++){
            std::cout << matrice[i][j];
            if (i < n-1) std::cout << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}