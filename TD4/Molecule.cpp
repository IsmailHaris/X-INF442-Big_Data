#include "Molecule.h"
#include <fstream>
#include <limits>
#include <cstdlib>

double squaredDistance(const double p[3], const double q[3]) {
	double x = 0.0;
	x += (p[0] - q[0]) * (p[0] - q[0]);
	x += (p[1] - q[1]) * (p[1] - q[1]);
	x += (p[2] - q[2]) * (p[2] - q[2]);
	return x;
}

Boule::Boule(const double c[3], const double r) {
	centre[0] = c[0];
	centre[1] = c[1];
	centre[2] = c[2];
	radius = r;
}

bool Boule::contains(const double pt[3]) {
	return squaredDistance(pt, centre) <= radius * radius;
}

// reads a molecule from a text file.
// File format: one line per Boule, each line in the form "x y z r"
// where (x,y,z) is the centre and r is the radius of the Boule
// No checking is performed here, we assume that the file is well-formed!
std::vector<Boule> readMolecule(const char *filename) {
	std::vector<Boule> mol;
	std::ifstream file(filename);
	double c[3], r;
	while (file >> c[0] >> c[1] >> c[2] >> r) {
		Boule b(c, r);
		mol.push_back(b);
	}
	return mol;
}

// Constructs a suitable bounding box for a molecule,
// storing it in the first argument
void boundingBox(double BB[3][2], std::vector<Boule> mol) {

	/* Avec des cubes :
	double min = std::numeric_limits<double>::infinity();
	double max = -std::numeric_limits<double>::infinity();
	int k=0;
	for(std::vector<Boule>::iterator it = mol.begin(); it != mol.end(); it++, k++) {
		for (int i=0; i<3; i++){	
			if(max < mol[k].centre[i]+mol[k].radius) { 
				max = mol[k].centre[i]+mol[k].radius;
			}
			if(min > mol[k].centre[i]-mol[k].radius) { 
				min = mol[k].centre[i]-mol[k].radius;
			}
		}
	}

	for (int i=0; i<3; i++){
		BB[i][0] = min;
		BB[i][1] = max;
	}*/

	/* Avec un variable itératrice
	int k=0;
	for(std::vector<Boule>::iterator it = mol.begin(); it != mol.end(); it++, k++) {
		for (int i=0; i<3; i++){	
			if(BB[i][1] < mol[k].centre[i]+mol[k].radius) { 
				BB[i][1] = mol[k].centre[i]+mol[k].radius;
			}
			if(BB[i][0] > mol[k].centre[i]-mol[k].radius) { 
				BB[i][0] = mol[k].centre[i]-mol[k].radius;
			}
		}
	}*/

	BB[0][0] = BB[1][0] = BB[2][0] = std::numeric_limits<double>::infinity(); // min
	BB[0][1] = BB[1][1] = BB[2][1] = -std::numeric_limits<double>::infinity(); // max

	// Avec les pointeurs : it->
	for(std::vector<Boule>::iterator it = mol.begin(); it != mol.end(); it++) {
		for (int i=0; i<3; i++){	
			if(BB[i][1] < it->centre[i]+ it->radius) { 
				BB[i][1] = it->centre[i]+ it->radius;
			}
			if(BB[i][0] > it->centre[i]- it->radius) { 
				BB[i][0] = it->centre[i]- it->radius;
			}
		}
	}


	/* Avec la valeur : (*it).
	for(std::vector<Boule>::iterator it = mol.begin(); it != mol.end(); it++) {
		for (int i=0; i<3; i++){	
			if(BB[i][1] < (*it).centre[i]+ (*it).radius) { 
				BB[i][1] = (*it).centre[i]+ (*it).radius;
			}
			if(BB[i][0] > (*it).centre[i]- (*it).radius) { 
				BB[i][0] = (*it).centre[i]- (*it).radius;
			}
		}
	}*/


}

// A random point in the given bounding box;
// the first argument is modified to hold the result.
// Uses rand( ) (which should be seeded externally).
void randomPoint(double pt[3], const double BB[3][2]) {
	// TODO: complete this function
	for (int i=0; i<3; i++){
		pt[i] = rand()/(double)RAND_MAX*(BB[i][1]-BB[i][0]) + BB[i][0];
	}
}
