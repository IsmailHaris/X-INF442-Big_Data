#include "Complex.hpp"

Complex::Complex() {
	a = 0.0;
	b = 0.0;
}

Complex::Complex(double a, double b){
	this->a = a;
	this->b = b;
}

double Complex::module(){
  return sqrt(pow(this->a,2)+pow(this->b,2));
}

Complex::~Complex() {}

std::ostream& operator<<(std::ostream& s, const Complex& c) {
 
    s << c.a;
    
    if(c.b > 0){
     s << "+" << c.b << "*i";
    }else if(c.b < 0){
     s << c.b << "*i";
    }
     
    return s;
     
}