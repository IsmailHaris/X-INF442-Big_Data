#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "spiral.hpp"
#include "Cell.hpp"
#include <iomanip>
#include <stdio.h>

using namespace std;

Cell* Memory::getCellByCoords(int col, int row) {
	if (col+offx>=0 and col+offx<side and row+offy>=0 and row+offy<side)
		return mem[col+offx][row+offy];
	else
		return NULL;
}

Cell* Memory::getCellById(int id) {
	if (cells.count(id)>0) return cells[id];
	else return NULL;
}

void Memory::printGrid() {
	for(int i=0;i<this->side;i++){
		for(int j=0;j<this->side;j++){
			std::cout << getCellByCoords(i,j)->getId() << std::endl;
			//printf(getCellByCoords(i,j)->read(),"d");
			//printf("  ","d");
		}
	}

}

Memory::Memory(int side) {
	this->side = side;
	this->offx = (side-1)/2;
	this->offy = side/2;
	
	this->mem = new Cell**[side];

	// Create cells
	for (int i=0, k=0; i< side; i++) {	
		this->mem[i] = new Cell*[side];
		for (int j=0; j<side; j++, k++) {
			this->mem[i][j] = new Cell(i-this->offx,j-this->offy); // initialization
			//cout << "Created cell on mem["<< i << "][" << j << "], with coords x=" << i-this->offx << " y=" << j-this->offy << endl; 
		}
	}

	// Set neighbors (private vars up, down, left, right)
	for (int i=0; i< side; i++) {	
		for (int j=0; j<side; j++) {
			Cell *up, *down, *left, *right;
			if (j>0) { up = this->mem[i][j-1]; } else { up = NULL; }
			if (j<side-1) { down = this->mem[i][j+1]; } else { down = NULL; }
			if (i>0) { left = this->mem[i-1][j]; } else { left = NULL; }
			if (i<side-1) { right = this->mem[i+1][j]; } else { right = NULL; }
			this->mem[i][j]->setNeighbors(up, down, left, right); 
		}
	}

	// Initialize mem values
	Cell* p = mem[this->offx][this->offy];
	for (int k=0; k < side*side; k++) {	
		if (p) {
			p->setId(k+1);		// set id in spiral
			p->set(0);			// initialize mem value to 0
			this->cells[k+1] = p;
			p = p->MoveToNext();
		}
		else {
			cout << "Error : cannot move next (k="<<k<<", offx="<<this->offx<<", offy="<<this->offy<<")." << endl;
			exit(-1);
		}
	} 
	p = mem[this->offx][this->offy];
}
