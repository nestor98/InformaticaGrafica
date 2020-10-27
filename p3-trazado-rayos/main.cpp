#include <iostream>
#include <memory>
#include "escena.hpp"
// #include "camara.cpp"
#include "esfera.hpp"
#include "plano.hpp"

#include "../p1-geometria/Vector3.cpp"
//#include "Matriz4.cpp"

using namespace std;


/**************** Programa principal ****************/
int main(int argc, char* argv[]) {
	if (argc < 0) {
		cerr << "invocar como:\n" << argv[0] << " <fichero de entrada> <fichero de salida>\n";
		exit(1);
	}
	Vector3 posCam(0,0,0,true);
	Vector3 dirCam(0,1,0,false);
	//Camara c(posCam, dirCam);
	//cout << c << endl;

	Escena e(Camara(posCam, dirCam));
	Vector3 posEsf(0,10,0,true);
	Esfera esf(posEsf, 4);
	e.addFigura(std::make_shared<Esfera>(esf));
	e.addFigura(std::make_shared<Plano>(Plano(posEsf, -dirCam))); // plano en la pos de la esfera con la normal hacia la camara
	cout << e << endl;
	//Escena e()
}
