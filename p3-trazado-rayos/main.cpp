#include <iostream>
#include <memory>
#include "escena.hpp"
// #include "camara.cpp"
#include "esfera.hpp"
#include "plano.hpp"
#include "prisma.hpp"

#include "Vector3.hpp"
//#include "Matriz4.cpp"


#include "generador.hpp"

using namespace std;




void escenaBastanteGuay400prismas200esferas(char* argv[]) {

	// std::shared_ptr<Utils> utils = make_shared<Utils>(Utils());
	int pixelesX = 720;
	int pixelesY = 720*9/16;
	Vector3 posCam(0,0,0,true);
	Vector3 fCam(0,8,0,false);
	Vector3 lCam(1,0,0,false);
	Vector3 uCam(0,0,double(pixelesY)/double(pixelesX),false);
	//Camara c(posCam, dirCam);
	//cout << c << endl;
	int rayosPP = 5; // rayos por pixel
	Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);

	int nThreads = 16; // TODO: CAMBIAR!!!!!!!!!!!!!!!!!

	Escena e(std::make_shared<Camara>(c), nThreads);
	Vector3 posEsf/*(0,3000,0,true)*/=FRONT*1000;
	//PRUEBAS TEXTURAS SIMPLE
	Imagen t= Imagen("textura3.ppm", true);
	//Textura tex=Textura(t, 474.0, 474.0, posEsf-2.0*uCam+lCam+5.0*(0.2*fCam));
	// std::make_shared<Esfera>(Esfera(algo)) ===  std::shared_ptr<Esfera>(new Esfera(algo))
	for(int i=0; i<20; i++){
		double tam=50.0/12;
		Esfera esf(posEsf+LEFT*(i-10)*tam*3+FRONT*(i-10)*tam/10.0, tam);// 1*1
		e.addFigura(std::make_shared<Esfera>(esf));
	}
	Plano p(-FRONT, 1000);
	p.setColor(0.5,0.5,0.5);
	e.addFigura(std::make_shared<Plano>(p));
	// for (int i=0; i<200; i++) {
	// 	// Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
	// 	Esfera esf(posEsf-2.0*uCam+lCam+5.0*(0.2*i*fCam), 0.5);// 1*1
	//
	// 	// cout << esf.to_string() << endl;
	// 	esf.setRandomColor();
	// 	e.addFigura(std::make_shared<Esfera>(esf));
	// 	// cout << "----\n";
	// }
	// // for (int i=0; i<1; i++) {
	// // 	// Esfera esf(posEsf+5.0*(0.2*(i+1)*fCam)+(0.3*i*lCam), 0.5);
	// // 	Esfera esf(posEsf+4*(0.3*(i+1)*lCam), 0.5);
	// //
	// // 	// cout << esf.to_string() << endl;
	// // 	esf.setRandomColor();
	// // 	e.addFigura(std::make_shared<Esfera>(esf));
	// // 	// cout << "----\n";
	// // }
	// // Suelo (normal, distancia al origen):
	// // Plano suelo(uCam/uCam.getModulo(), 3);
	// // suelo.setColor(0,0,0.8);
	// // e.addFigura(std::make_shared<Plano>(suelo));
	// for (int i=0; i<400; i++) {
	// 	Vector3 posPrisma = posEsf + lCam*3+ 2*fCam*(i-10)/3.0 - lCam*4*(i%2);// a la dcha de la esfera
	// 	Vector3 tamPrisma(1-0.5*(i%2),1,3,false);
	// 	Prisma prisma(posPrisma, tamPrisma);
	// 	prisma.setRandomColor();
	// 	// prisma.setColor(0,0.7,0);
	// 	e.addFigura(std::make_shared<Prisma>(prisma));
	// }
	//e.addFigura(std::make_shared<Plano>(Plano(posEsf, -fCam))); // plano en la pos de la esfera con la normal hacia la camara

	// cout<<e<<endl;
	e.render("out/" + string(argv[1]));
	//e.testBVHRender();
	cout << "en total hay "<<pixelesX*pixelesY<<endl;
}


void escenaEsponja(char* argv[]) {

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
		Vector3 posEsponja(-2,300,-10.5,true);
		Vector3 tamEsponja(30,30,30,false);
		GeneradorEstructuras gen(GeneradorEstructuras::Estructura::MengerSponge, posEsponja, tamEsponja, 4);
		auto figuras = gen.getVectorFiguras(); // Devuelve un puntero al vector de las figuras
		e.addFiguras(figuras);
		Plano suelo(uCam/uCam.getModulo(), 3);
		// suelo.setColor(0,0,0.8);
		e.addFigura(std::make_shared<Plano>(suelo));
		e.render("out/" + string(argv[1]));
		std::cout << "escena\n" <<e << '\n';
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




void escenaPlanos(char* argv[]) {

		int pixelesX = 720;
		int pixelesY = 720;//*9/16;
		Vector3 posCam(0,0,0,true);
		Vector3 fCam(0,1,0,false);
		Vector3 lCam(1,0,0,false);
		Vector3 uCam(0,0,double(pixelesY)/double(pixelesX),false);
		cout << "mod fcam: " << fCam.getModulo() << endl;
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		int rayosPP = 50; // rayos por pixel
		Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		c.setFOV(3.0*PI/2.0);

		int nThreads = 16; // TODO: CAMBIAR!!!!!!!!!!!!!!!!!
		//
		Escena e(std::make_shared<Camara>(c), nThreads);

		float distanciaParedes = 3;
		Plano suelo(uCam/uCam.getModulo(), distanciaParedes);
		suelo.setColor(0.8,0.8,0.8);
		e.addFigura(std::make_shared<Plano>(suelo));
		Plano techo(-uCam/uCam.getModulo(), distanciaParedes);
		techo.setColor(0.8,0.8,0.8);
		e.addFigura(std::make_shared<Plano>(techo));
		Plano paredi(lCam/lCam.getModulo(), distanciaParedes);
		paredi.setColor(0.8,0,0);
		e.addFigura(std::make_shared<Plano>(paredi));
		Plano paredd(-lCam/lCam.getModulo(), distanciaParedes);
		paredd.setColor(0,0.8,0);
		e.addFigura(std::make_shared<Plano>(paredd));
		Plano paredFondo(-fCam/fCam.getModulo(), 2*distanciaParedes);
		paredFondo.setColor(0.75,0.75,0.75);
		e.addFigura(std::make_shared<Plano>(paredFondo));
		e.render("out/" + string(argv[1]));
		// e.testBVHRender();
		std::cout << "escena\n" <<e << '\n';
}


void escenaCornellBox2(char* argv[]) {

		int pixelesX = 400;
		int pixelesY = 400;//*9/16;
		Vector3 posCam(0,0,0,true);
		Vector3 fCam = FRONT;//(0,1,0,false);
		Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		cout << "mod fcam: " << fCam.getModulo() << endl;
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		int rayosPP = 50; // rayos por pixel
		Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		c.setFOV(0.4*PI);
		// c.setFOV(PI);

		int nThreads = 16; // TODO: CAMBIAR!!!!!!!!!!!!!!!!!
		//
		Escena e(std::make_shared<Camara>(c), nThreads, Escena::TipoRender::Distancia);

		// std::cout << "front x left = " << cross(FRONT, LEFT) << "\nUP = " << UP << endl;
		double d_prisma = 32;// * 10.0;
		for (int i = 0; i<100; i++) {
			Vector3 tamPrisma(1,1,1, false);
			tamPrisma = d_prisma/2.0 * tamPrisma;
			Prisma caja(0.5*LEFT + d_prisma * FRONT * (i-5), tamPrisma);// 1*1
			// cout << esf.to_string() << endl;
			caja.setRandomColor();
			e.addFigura(std::make_shared<Prisma>(caja));
		}

		e.render("out/" + string(argv[1]));
		// e.testBVHRender();
		std::cout << "escena\n" <<e << '\n';
}


void escenaCornellBox(char* argv[]) {

		int pixelesX = 400;
		int pixelesY = 400;//*9/16;
		Vector3 posCam(0,0,0,true);
		Vector3 fCam = FRONT;//(0,1,0,false);
		Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		cout << "mod fcam: " << fCam.getModulo() << endl;
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		int rayosPP = 1; // rayos por pixel
		Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		c.setFOV(0.4*PI);
		// c.setFOV(PI);

		int nThreads = 1; // TODO: CAMBIAR!!!!!!!!!!!!!!!!!
		//
		Escena e(std::make_shared<Camara>(c), nThreads, Escena::TipoRender::Normales);

		float distanciaParedes = 3;
		// Caja:
		Plano suelo(UP, distanciaParedes);
		suelo.setColor(0.8,0.8,0.8);
		e.addFigura(std::make_shared<Plano>(suelo));
		Plano techo(-UP, distanciaParedes);
		techo.setColor(0.8,0.8,0.8);
		e.addFigura(std::make_shared<Plano>(techo));
		Plano paredi(LEFT, distanciaParedes);
		paredi.setColor(0.8,0,0);
		e.addFigura(std::make_shared<Plano>(paredi));
		Plano paredd(-LEFT, distanciaParedes);
		paredd.setColor(0,0.8,0);
		e.addFigura(std::make_shared<Plano>(paredd));
		Plano paredFondo(-FRONT, 2.0*distanciaParedes);
		paredFondo.setColor(0.75,0.75,0.75);
		e.addFigura(std::make_shared<Plano>(paredFondo));
		// Figuras:
		// Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 centroSuelo = 1.5*distanciaParedes*FRONT - distanciaParedes*UP;
		for (int i = 0; i<1; i++) {
			float tamEsfera =distanciaParedes/3.0*1.2;

			Esfera esf(centroSuelo + tamEsfera*UP + 0.45*distanciaParedes*LEFT, tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esf));

			Esfera esfdcha(centroSuelo + tamEsfera*FRONT+tamEsfera*UP - 0.45*distanciaParedes*LEFT, 1.25*tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esfdcha.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esfdcha));
			Esfera esf2(centroSuelo /*+ distanciaParedes*UP*/, tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf2.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esf2));
			Vector3 tamPrisma(2.0*tamEsfera, 2.0*tamEsfera,2.0*tamEsfera, false);
			Prisma caja(centroSuelo - 0.5*distanciaParedes*LEFT, tamPrisma/2.0);// 1*1
			// cout << esf.to_string() << endl;
			caja.setRandomColor();
			//e.addFigura(std::make_shared<Prisma>(caja));
		}
		// std::cout << "front x left = " << cross(FRONT, LEFT) << "\nUP = " << UP << endl;
		double d_prisma = 2.0*distanciaParedes / 10.0;
		for (int i = 0; i<100; i++) {

			Vector3 tamPrisma(1,1,1, false);
			tamPrisma = d_prisma/2.0 * tamPrisma;
			Prisma caja(centroSuelo + 0.75*distanciaParedes*LEFT + d_prisma * FRONT * (i-5), tamPrisma);// 1*1
			// cout << esf.to_string() << endl;
			caja.setRandomColor();
			e.addFigura(std::make_shared<Prisma>(caja));
		}

		// e.render("out/" + string(argv[1]));
		e.testBVHRender();
		//std::cout << "escena\n" <<e << '\n';
}


void escenaCornellBoxMateriales(char* argv[]) {

		int pixelesX = 400;
		int pixelesY = 400;//*9/16;
		Vector3 posCam(0,0,0,true);
		Vector3 fCam = FRONT;//(0,1,0,false);
		Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		cout << "mod fcam: " << fCam.getModulo() << endl;
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		int rayosPP = 50; // rayos por pixel
		Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		c.setFOV(0.4*PI);
		// c.setFOV(PI);

		int nThreads = 16; // TODO: CAMBIAR!!!!!!!!!!!!!!!!!
		//
		Escena e(std::make_shared<Camara>(c), nThreads, Escena::TipoRender::Normales);

		float distanciaParedes = 3;

		Material difuso(Material::Difuso);
		Material difusoVerde = difuso;
		difusoVerde.setCoeficiente(Color(0,0.8,0),0);
		Material difusoRojo = difuso;
		difusoRojo.setCoeficiente(Color(0.8,0,0),0);
		Material difusoGris = difuso;
		difusoGris.setCoeficiente(Color(0.75,0.75,0.75),0);

		// Caja:
		Plano suelo(UP, distanciaParedes);
		// suelo.setColor(0.8,0.8,0.8);
		suelo.setMaterial(difusoGris);
		e.addFigura(std::make_shared<Plano>(suelo));
		Plano techo(-UP, distanciaParedes);
		techo.setMaterial(difusoGris);
		e.addFigura(std::make_shared<Plano>(techo));
		Plano paredi(-LEFT, distanciaParedes);
		// paredi.setColor(0.8,0,0);
		paredi.setMaterial(difusoVerde);
		e.addFigura(std::make_shared<Plano>(paredi));
		Plano paredd(LEFT, distanciaParedes);
		paredd.setMaterial(difusoRojo);
		// paredd.setColor(0,0.8,0);
		e.addFigura(std::make_shared<Plano>(paredd));
		Plano paredFondo(-FRONT, 2.0*distanciaParedes);
		// paredFondo.setColor(0.75,0.75,0.75);
		paredFondo.setMaterial(difusoGris);
		e.addFigura(std::make_shared<Plano>(paredFondo));


		Plano paredOculta(FRONT, distanciaParedes);
		paredOculta.setColor(0.65,0.65,0.65);
		e.addFigura(std::make_shared<Plano>(paredOculta));
		// Figuras:
		// Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 centroSuelo = 1.5*distanciaParedes*FRONT - distanciaParedes*UP;

		for (int i = 0; i<1; i++) {
			float tamEsfera =distanciaParedes/3.0*1.2;

			Esfera esf(centroSuelo + tamEsfera*UP + 0.45*distanciaParedes*LEFT, tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf.setMaterial(difuso);
			e.addFigura(std::make_shared<Esfera>(esf));
			//
			Esfera esfdcha(centroSuelo + tamEsfera*FRONT+tamEsfera*UP - 0.45*distanciaParedes*LEFT, 1.25*tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esfdcha.setMaterial(difuso);
			// esfdcha.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esfdcha));
			Esfera esf2(centroSuelo /*+ distanciaParedes*UP*/, tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf2.setMaterial(difuso);
			//esf2.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esf2));
			// Vector3 tamPrisma(2.0*tamEsfera, 2.0*tamEsfera,2.0*tamEsfera, false);
			// Prisma caja(centroSuelo - 0.5*distanciaParedes*LEFT, tamPrisma/2.0);// 1*1
			// // cout << esf.to_string() << endl;
			// caja.setRandomColor();
			// e.addFigura(std::make_shared<Prisma>(caja));
		}
		Vector3 tamPrismaLuz = -LEFT + FRONT + UP / 15.0;
		tamPrismaLuz = tamPrismaLuz*3;
		Vector3 posPrismaLuz = centroSuelo + UP * distanciaParedes * 1.95 + LEFT * tamPrismaLuz[0]/2.0;
		Prisma luz(posPrismaLuz, tamPrismaLuz);
		luz.setColor(1,1,1);
		e.addFigura(std::make_shared<Prisma>(luz));
		// std::cout << "front x left = " << cross(FRONT, LEFT) << "\nUP = " << UP << endl;
		// double d_prisma = 2.0*distanciaParedes / 10.0;
		// for (int i = 0; i<100; i++) {
		//
		// 	Vector3 tamPrisma(1,1,1, false);
		// 	tamPrisma = d_prisma/2.0 * tamPrisma;
		// 	Prisma caja(centroSuelo + 0.75*distanciaParedes*LEFT + d_prisma * FRONT * (i-5), tamPrisma);// 1*1
		// 	// cout << esf.to_string() << endl;
		// 	caja.setRandomColor();
		// 	e.addFigura(std::make_shared<Prisma>(caja));
		// }

		e.render("out/" + string(argv[1]));
		//e.testBVHRender();
		//std::cout << "escena\n" <<e << '\n';
}


/**************** Programa principal ****************/
int main(int argc, char* argv[]) {
	if (argc < 2) {// <fichero de entrada>
		cerr << "invocar como:\n" << argv[0] << " <fichero de salida>\n";
		exit(1);
	}
	// escenaBastanteGuay400prismas200esferas(argv);
	// escenaEsponja(argv);
	// escenaPlanos(argv);
	//escenaBastanteGuay400prismas200esferas(argv);
	escenaCornellBoxMateriales(argv);
}
