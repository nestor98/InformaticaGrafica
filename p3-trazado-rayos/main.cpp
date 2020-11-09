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

	int nThreads = 16; // TODO: CAMBIAR!!!!!!!!!!!!!!!!!

	Escena e(std::make_shared<Camara>(c), nThreads);
	Vector3 posEsf(-2,30,-0.5,true);
	for (int i=0; i<200; i++) {
		// Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Esfera esf(posEsf-2.0*uCam+lCam+5.0*(0.2*i*fCam), 0.5);// 1*1

		// cout << esf.to_string() << endl;
		esf.setRandomColor();
		e.addFigura(std::make_shared<Esfera>(esf));
		// cout << "----\n";
	}
	// for (int i=0; i<1; i++) {
	// 	// Esfera esf(posEsf+5.0*(0.2*(i+1)*fCam)+(0.3*i*lCam), 0.5);
	// 	Esfera esf(posEsf+4*(0.3*(i+1)*lCam), 0.5);
	//
	// 	// cout << esf.to_string() << endl;
	// 	esf.setRandomColor();
	// 	e.addFigura(std::make_shared<Esfera>(esf));
	// 	// cout << "----\n";
	// }
	// Suelo (normal, distancia al origen):
	// Plano suelo(uCam/uCam.getModulo(), 3);
	// suelo.setColor(0,0,0.8);
	// e.addFigura(std::make_shared<Plano>(suelo));
	for (int i=0; i<400; i++) {
		Vector3 posPrisma = posEsf + lCam*3+ 2*fCam*(i-10)/3.0 - lCam*4*(i%2);// a la dcha de la esfera
		Vector3 tamPrisma(1-0.5*(i%2),1,3,false);
		Prisma prisma(posPrisma, tamPrisma);
		prisma.setRandomColor();
		// prisma.setColor(0,0.7,0);
		e.addFigura(std::make_shared<Prisma>(prisma));
	}
	// cout<<e<<endl;
	e.render("out/" + string(argv[1]));
	cout << "en total hay "<<pixelesX*pixelesY<<endl;
	//e.addFigura(std::make_shared<Plano>(Plano(posEsf, -fCam))); // plano en la pos de la esfera con la normal hacia la camara
	// cout << e << endl;
	// for (int i = 47; i<49/*pixelesX/4*/; i++) {
	// 	int despl = pixelesX/6;
	// 	cout << "::::::::::::::::::::::::: PIXEL " << i + despl << endl;
	// 	for (int j = 0; j<1; j++) {
	// 		double t = prisma.interseccion(c.getPos(), c.getRayoPixel(i+despl));//200+pixelesX*pixelesY*1.0/3.0));
	// 		cout <<t << " ---------------------------------------------" << i << endl;
	// 	}
	//
	// }
	//Escena e()
}
