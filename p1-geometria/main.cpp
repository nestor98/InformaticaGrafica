#include <iostream>
#include "Vector3.cpp"
//#include "Matriz4.cpp"

using namespace std;

int main() {
	
	Vector3 v1(2, 2, 2, false);
	Matriz4 m(v1, v1, v1, v1);

	cout << 5.0 * v1 << endl;
	//cout << "hey" << endl;
	Vector3 v3 = m*v1;

	//cout << "hearsljnagsnon" << endl;
	cout << v3<< endl;
	
	return 0;
}