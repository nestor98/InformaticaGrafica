#include <iostream>
#include "Imagen.cpp"
//#include "Matriz4.cpp"

using namespace std;


void probarClamp(Imagen& i) {
	cout << "Imagen antes de clamp:\n" << i << endl;
	i.clamp();
	cout << "Despues\n:" << i << endl;
}


void probarEqualization(Imagen& i1) {
	cout << "Imagen antes de eq:\n" << i1 << endl;
	i1.equalize();
	cout << "Despues\n:" << i1 << endl;
}

/**************** Programa principal ****************/
int main() {

	Imagen i1("hdrFiles/mpi_office.ppm");
	i1.guardar("procesados/mpi_office_deberiaserigual.ppm");
	Imagen i2(i1);
	cout << "i2:\n" << i2 << endl; 
	/*probarClamp(i1);
	probarClamp2(i2);
	*/
	probarEqualization(i1);
	i1.guardar("procesados/mpi_office_ecualizado_2.ppm");
}