#include <iostream>
#include "Imagen.cpp"
//#include "Matriz4.cpp"

using namespace std;


/**************** Programa principal ****************/
int main() {
/*
	cout << " arraysssssssssssssss " <<endl;
	array<int, 5> a;
	a[3] = 2;
	/*for (auto e : a) {
		cout << e << endl;
	}


	vector<array<int, 5>> v;
	v.reserve(10);
	/*
	v[0][0] = 10;
	v[3][1] = 10;
	cout << v[0][0] << endl;
	for (int i = 0; i< 10; i++) {
		cout << "recorriendo v\n";
		for (auto ea : v[i]) {
			cout << ea << endl;
		}
	}

	cout << "</arraysssssssssssssss>" << endl;
	*/

	Imagen i1("hdrFiles/mpi_office.ppm");
	i1.guardar("procesados/mpi_office_deberiaserigual.ppm");
	//cout << "Imagen:\n" << i1 << endl;
}