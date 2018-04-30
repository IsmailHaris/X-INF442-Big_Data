#include "QuadTree.hpp"
#include <iostream>

QuadTree::QuadTree(std::vector<Atom*>& v, Rectangle& r) {

	rectangle = r;

	if (v.size() == 0) {

		topLeft = 0;
		topRight = 0;
		bottomLeft = 0;
		bottomRight = 0;

		atom = 0;

	}
	else if (v.size() == 1) {

		topLeft = 0;
		topRight = 0;
		bottomLeft = 0;
		bottomRight = 0;

		atom = v[0];

	}
	else {

		atom = 0;
		
		std::vector<Atom*> v1;
		std::vector<Atom*> v2;
		std::vector<Atom*> v3;
		std::vector<Atom*> v4;
		
		Rectangle r1 = r.topLeftRectangle();
		Rectangle r2 = r.topRightRectangle();
		Rectangle r3 = r.bottomLeftRectangle();
		Rectangle r4 = r.bottomRightRectangle();
		
		for(int i=0; i<v.size(); i++){
		  
		  if(r1.contains(v[i])) v1.push_back(v[i]);
		  else if(r2.contains(v[i])) v2.push_back(v[i]);
		  else if(r3.contains(v[i])) v3.push_back(v[i]);
		  else if(r4.contains(v[i])) v4.push_back(v[i]);
		
		}

		// new renvoie un pointeur memoire vers l'objet créé donc pas besoin d'étoile devant topLeft etc
		// (qui sont déjà un pointeurs déclarés dans le hpp)
		topLeft = new QuadTree(v1, r1);
		topRight = new QuadTree(v2, r2);
		bottomLeft = new QuadTree(v3, r3);
		bottomRight = new QuadTree(v4, r4);
	}

}

void QuadTree::print(unsigned int offset) const {

	if (topLeft) {

		for (unsigned int i = 0; i < offset; i++) std::cout << "\t";
		std::cout << "topLeft";
		std::cout << std::endl;
		topLeft->print(offset + 1);

		for (unsigned int i = 0; i < offset; i++) std::cout << "\t";
		std::cout << "topRight";
		std::cout << std::endl;
		topRight->print(offset + 1);

		for (unsigned int i = 0; i < offset; i++) std::cout << "\t";
		std::cout << "bottomLeft";
		std::cout << std::endl;
		bottomLeft->print(offset + 1);

		for (unsigned int i = 0; i < offset; i++) std::cout << "\t";
		std::cout << "bottomRight";
		std::cout << std::endl;
		bottomRight->print(offset + 1);

	}
	else if (atom) {

		for (unsigned int i = 0; i < offset; i++) std::cout << "\t";
		std::cout << "Atom = " << atom->x << " " << atom->y << std::endl;

	}

}

void QuadTree::rangeSearch(std::vector<Atom*>& v, Rectangle& r) {

	if (r.intersects(rectangle)){
		if (r.contains(atom)){
			v.push_back(atom);
		}
		else if (topLeft) {
			topLeft->rangeSearch(v, r);
			topRight->rangeSearch(v, r);
			bottomLeft->rangeSearch(v, r);
			bottomRight->rangeSearch(v, r);
		}
	}
}

QuadTree::~QuadTree() {

	if (topLeft) delete topLeft;
	if (topRight) delete topRight;
	if (bottomLeft) delete bottomLeft;
	if (bottomRight) delete bottomRight;

}