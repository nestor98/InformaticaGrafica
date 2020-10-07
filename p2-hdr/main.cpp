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

void probarGamma(Imagen& i1) {
	cout << "Imagen antes de gamma:\n" << i1 << endl;
	i1.gamma(-10);
	cout << "Despues\n:" << i1 << endl;
}

/**************** Programa principal ****************/
int main(int argc, char* argv[]) {
	if (argc < 3) {
		cerr << "invocar como:\n" << argv[0] << " <fichero de entrada> <fichero de salida>\n";
		exit(1);
	}
	Imagen i1(argv[1]);
	//i1.guardar("procesados/mpi_office_ldr.ppm", true);
	//Imagen i2(i1);
	//cout << "i2:\n" << i2 << endl; 
	/*probarClamp(i1);
	probarClamp2(i2);
	*/
	probarEqualization(i1);
	
	i1.guardar(argv[2], false);
	/*probarGamma(i1);
	i1.guardar("procesados/mpi_office_gamma_ldr.ppm", false);*/
}