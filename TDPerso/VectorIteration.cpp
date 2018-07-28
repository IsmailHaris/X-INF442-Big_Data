#include <iostream>
#include <vector>
 
int main (int argc, char *argv[]) {

	using namespace std;  

	vector<string> myvector;  // a vector of stings.


	// push some strings in the vector.
	myvector.push_back("a");
	myvector.push_back("b");
	myvector.push_back("c");
	myvector.push_back("d");


	vector<string>::iterator it;  // declare an iterator to a vector of strings
	int n = 3;  // nth element to be found.
	int i = 0;  // counter.

	// now start at from the beginning
	// and keep iterating over the element till you find
	// nth element...or reach the end of vector.
	for(it = myvector.begin(); it != myvector.end(); it++,i++ )    {
	    // found nth element..print and break.
	    if(i == n) {
	        cout<< *it << endl;  // prints d.
	        break;
	    }
	}

	// other easier ways of doing the same.
	// using operator[]
	cout<<myvector[n]<<endl;  // prints d.

	// using the at method
	cout << myvector.at(n) << endl;  // prints d.

}