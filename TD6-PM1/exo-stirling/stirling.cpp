#include <iostream>
#include <math.h>
#include "stirling.hpp"

using namespace std;

long Factorial(int n) {
	if(n<=0){
		return 1;
	}
	// TODO, exercise stirling : implement this function (and return the corresponding value)
	return n*Factorial(n-1);
}

long Binomial(int n, int k) {
	return Factorial(n)/(Factorial(n-k)*Factorial(k));
}

long Stirling2kind_Explicit(int n, int k) {
	// TODO, exercise stirling : implement this function (and return the corresponding value)
	long sum=0;

	for(int j=0; j<=k;j++){
		sum += pow(-1,k-j)*Binomial(k,j)*pow(j,n);
	}

	return sum/long(Factorial(k));
}

long Stirling2kind_Recurrence(int n, int k, int& ncalls) {
	// TODO, exercise stirling : implement this function (and return the corresponding value)
	ncalls++;
	if (k==0 && n==0){
		return 1;
	}
	if (k==0 && n>0){
		return 0;
	}
	if (k>0 && n==0){
		return 0;
	}
	return k*Stirling2kind_Recurrence(n-1,k,ncalls)+Stirling2kind_Recurrence(n-1,k-1,ncalls);
}
