#include <iostream>
#include <memory>
#include "escena.hpp"
// #include "camara.cpp"
#include "esfera.hpp"
#include "plano.hpp"
#include "prisma.hpp"

#include "Vector3.hpp"
//#include "Matriz4.cpp"

using namespace std;

/**************** Programa principal ****************/
int main(int argc, char* argv[]) {
	if (argc < 2) {// <fichero de entrada>
		cerr << "invocar como:\n" << argv[0] << " <fichero de salida>\n";
		exit(1);
	}
	int pixelesX = 720;
	int pixelesY = 720*9/16;
	Vector3 posCam(0,0,0,true);
	Vector3 fCam(0,8,0,false);
	Vector3 lCam(1,0,0,false);
	Vector3 uCam(0,0,double(pixelesY)/double(pixelesX),false);
	//Camara c(posCam, dirCam);
	//cout << c << endl;
	int rayosPP = 50; // rayos por pixel
	Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
	Escena e(std::make_shared<Camara>(c));
	Vector3 posEsf(-2,30,0,true);
	for (int i=0; i<100; i++) {
		Esfera esf(posEsf+(0.2*i*fCam+0.3*i*uCam), 0.5);
		cout << esf.to_string() << endl;
		esf.setRandomColor();
		e.addFigura(std::make_shared<Esfera>(esf));
		cout << "----\n";
	}
	for (int i=0; i<50; i++) {
		Esfera esf(posEsf+(0.2*i*fCam)+(0.3*i*lCam), 0.5);
		cout << esf.to_string() << endl;
		esf.setRandomColor();
		e.addFigura(std::make_shared<Esfera>(esf));
		cout << "----\n";
	}
	// Suelo (normal, distancia al origen):
	Plano suelo(uCam/uCam.getModulo(), 3);
	suelo.setColor(0,0,0.8);
	e.addFigura(std::make_shared<Plano>(suelo));
	Vector3 posPrisma = posEsf + lCam*3 + uCam;// a la dcha de la esfera
	Vector3 tamPrisma(1,50,0.5,false);
	Prisma prisma(posPrisma, 2*tamPrisma);
	prisma.setColor(0,0.7,0);
	e.addFigura(std::make_shared<Prisma>(prisma));
	cout<<e<<endl;
	e.render("out/" + string(argv[1]));
	cout << "en total hay "<<pixelesX*pixelesY<<endl;
	//e.addFigura(std::make_shared<Plano>(Plano(posEsf, -fCam))); // plano en la pos de la esfera con la normal hacia la camara
	// cout << e << endl;
	// for (int i = 0; i<100; i++) {
	// 	for (int j = 0; j<rayosPP; j++) {
	// 		double t = prisma.interseccion(c.getPos(), c.getRayoPixel(i+200+pixelesX*pixelesY*1.0/3.0));
	// 		cout <<t << " ---------------------------------------------"<< endl;
	// 	}
	//
	// }
	//Escena e()
}
