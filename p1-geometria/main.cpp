#include <iostream>
#include "Vector3.cpp"
//#include "Matriz4.cpp"

using namespace std;

int main() {
	
	Vector3 v1(2, 2, 2, false);
	Matriz4 m(v1, v1, v1, v1);

	cout << (5.0 * v1).to_string() << endl;
	//cout << "hey" << endl;
	Vector3 v3 = v1 * m;

	//cout << "hearsljnagsnon" << endl;
	cout << v3.to_string()<< endl;
	
	return 0;
}