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

void probarGamma(Imagen& i1, const float g) {
	cout << "Imagen antes de gamma:\n" << i1 << endl;
	i1.gamma(g);
	cout << "Despues\n:" << i1 << endl;
}

void probarGammaClamp(Imagen& i1, const float g, const float max) {
	cout << "Imagen antes de gamma:\n" << i1 << endl;
	i1.gammaClamp(g, max);
	cout << "Despues\n:" << i1 << endl;
}

void probarEqualizationClamp(Imagen& i1) {
	cout << "Imagen antes de eq:\n" << i1 << endl;
	i1.equalizeAndClamp(0.8);
	cout << "Despues\n:" << i1 << endl;
}


void probarReinhard(Imagen& i1) {
	cout << "Imagen antes de eq:\n" << i1 << endl;
	i1.reinhard();
	cout << "Despues\n:" << i1 << endl;
}


void probarExtendedReinhard(Imagen& i1) {
	cout << "Imagen antes de eq:\n" << i1 << endl;
	i1.extendedReinhard();
	cout << "Despues\n:" << i1 << endl;
}

/**************** Programa principal ****************/
int main(int argc, char* argv[]) {
	int ejecutar = 1; // ELEGIR PROGRAMA: 0 el original de la p2, 1 para comparar imagenes
	if (ejecutar == 0) {
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
		//probarEqualizationClamp(i1);
		cout << "maxfloat antes: " << i1.getMaxFloat() << endl;
		//probarGammaClamp(i1, 0.75, 0.95);
		probarExtendedReinhard(i1);
		cout << "maxfloat despues: " << i1.getMaxFloat() << endl;
		i1.guardar(string(argv[2]), false);
		/*probarGamma(i1);
		i1.guardar("procesados/mpi_office_gamma_ldr.ppm", false);*/
	}
	else if (ejecutar == 1) {
		if (argc < 4) {
			cerr << "invocar como:\n" << argv[0] << " <nSamples> <imagen1> <imagen2>\n";
			exit(1);
		}
		Imagen i1(argv[2], false);
		Imagen i2(argv[3], false);
		//std::cout << "i1:\n" << i1 << '\n' << "i2:\n" << i2 << endl;
		cout << argv[1] << "\t" << diferencia(i1, i2) << endl;
	}

}
