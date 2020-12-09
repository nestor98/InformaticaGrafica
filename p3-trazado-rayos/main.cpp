#include <iostream>
#include <memory>
#include "escena.hpp"
// #include "camara.cpp"
#include "esfera.hpp"
#include "plano.hpp"
#include "prisma.hpp"

#include "Vector3.hpp"
//#include "Matriz4.cpp"
#include "renderer.hpp"
#include "pm-renderer.hpp"

#include "generador.hpp"

using namespace std;


/*
std::unique_ptr<Escena> escenaCornellBoxTexturas(const int pixelesX, const int pixelesY, const int rayosPP) {

		double distanciaParedes = 3;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		Vector3 fCam = FRONT;//(0,1,0,false);
		Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		// int rayosPP =500; // rayos por pixel

		double fov = PI/4;
		Camara c = Camara(posCam, uCam, fov, pixelesX,pixelesY,rayosPP);

		// c.setFOV(0.4*PI);
		// c.setFOV(PI);

		//

		Escena e(std::make_shared<Camara>(c));//Materiales);//MaterialesSinBVH
		//
		// // Caja:
		Plano suelo(UP, 1.0*distanciaParedes);
		suelo.setColor(0.8,0.8,0.8);
		e.addFigura(std::make_shared<Plano>(suelo));

		Plano techo(-UP, distanciaParedes);
		techo.setColor(1,1,1);
		// techo.setMaterial(difusoGris);
		e.addFigura(std::make_shared<Plano>(techo));
		Plano paredi(-LEFT, distanciaParedes);
		paredi.setColor(0.8,0,0);
		//paredi.setMaterial(difusoVerde);
		e.addFigura(std::make_shared<Plano>(paredi));
		Plano paredd(LEFT, distanciaParedes);

		//paredd.setMaterial(difusoRojo);
		 paredd.setColor(0,0.8,0);
		e.addFigura(std::make_shared<Plano>(paredd));
		Plano paredFondo(-FRONT, 2.0*distanciaParedes);
		 paredFondo.setColor(0.3,0.75,0.9);
		e.addFigura(std::make_shared<Plano>(paredFondo));
		//
		//
		Plano paredOculta(FRONT, 0.1*distanciaParedes);
		paredOculta.setColor(0.05,0.05,0.8);
		e.addFigura(std::make_shared<Plano>(paredOculta));
		// Figuras:
		//Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 centroSuelo = 1.5*distanciaParedes*FRONT - distanciaParedes*UP;
		for (int i = 0; i<1; i++) {
			float tamEsfera =distanciaParedes/3.0*1.2;

			Esfera esf(centroSuelo + 0.5*tamEsfera*UP + 0.45*distanciaParedes*LEFT, tamEsfera);// 1*1
			e.addFigura(std::make_shared<Esfera>(esf));
			//
			Vector3 posEsfDcha = centroSuelo - 1.0*tamEsfera*FRONT+ 0.5*tamEsfera*UP - 0.45*distanciaParedes*LEFT;
			Esfera esfdcha(posEsfDcha, 1.0*tamEsfera);// 1*1

			esfdcha.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esfdcha));

				Vector3 posEsf/*(0,3000,0,true)=FRONT*1000;
			//PRUEBAS TEXTURAS SIMPLE
			Imagen t= Imagen("textura1.ppm", true);
			Textura tex=Textura(t, tamEsfera*2, tamEsfera*2, centroSuelo + 0.5*tamEsfera*UP + 0.45*distanciaParedes*LEFT);
			Esfera esfT(centroSuelo + 0.5*tamEsfera*UP + 0.45*distanciaParedes*LEFT, tamEsfera, std::make_shared<Textura> (tex));// 1*1
			esfT.setMaterial(Material(Color(0,0.8,0), Color(), Color()));
			e.addFigura(std::make_shared<Esfera>(esfT));


		} // LUZ:
		// Vector3 tamPrismaLuz = -LEFT + FRONT + UP / 15.0;
		// tamPrismaLuz = tamPrismaLuz*30.0;
		// Vector3 posPrismaLuz = centroSuelo + UP * distanciaParedes * 1.95 + LEFT * tamPrismaLuz[0]/2.0 - FRONT*tamPrismaLuz[2]/2.0;
		// Prisma luz(posPrismaLuz, tamPrismaLuz);
		// luz.setColor(1,1,1);
		// e.addFigura(std::make_shared<Prisma>(luz));


		return std::make_unique<Escena>(e);
}*/

std::unique_ptr<Escena> escenaCornellBoxMateriales(const int pixelesX, const int pixelesY, const int rayosPP) {
		// ----------------------- Constantes de la escena:
		double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		Vector3 fCam = FRONT;//(0,1,0,false);
		Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		// int rayosPP =500; // rayos por pixel
		Imagen t= Imagen("textura1.ppm", true);
		Textura tex=Textura(t,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);


		// ----------------------- Camara:
		// Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		// std::cout << gradosARad(90) << '\n'<< PI/4.0 <<'\n';
		double fov = gradosARad(90); //0.475 * PI;
		Camara c = Camara(posCam, centroHabitacion, uCam, fov, pixelesX, pixelesY, rayosPP);


		//

		Escena e(std::make_shared<Camara>(c));//Materiales);//MaterialesSinBVH



		Material difuso(Material::Difuso);
		Material difusoVerde = difuso;
		difusoVerde.setCoeficiente(Color(0,0.8,0),0);
		Material difusoRojo = difuso;
		difusoRojo.setCoeficiente(Color(0.8,0,0),0);
		Material difusoGris = DIFUSO_GRIS;
		//
		// Caja:
		Plano suelo(UP, 1.0*distanciaParedes);
		// suelo.setColor(0.8,0.8,0.8);
		Textura texRotada = tex;
		Matriz4 rotaciontex;
		rotaciontex.setRotarX(gradosARad(90));
		texRotada.rotar(rotaciontex);
		suelo.setTextura(std::make_shared<Textura>(texRotada));
		suelo.setMaterial(difusoGris);
		e.addFigura(std::make_shared<Plano>(suelo));

		Plano techo(-UP, distanciaParedes);
		// techo.setColor(2.5,2.5,2.5);
		techo.setMaterial(DIFUSO_BLANCO);
		e.addFigura(std::make_shared<Plano>(techo));
		Plano paredi(-LEFT, distanciaParedes);
		 // paredi.setColor(0.8,0,0);
		paredi.setMaterial(difusoRojo);
		e.addFigura(std::make_shared<Plano>(paredi));
		Plano paredd(LEFT, distanciaParedes);

		paredd.setMaterial(ESPEJO);
		 // paredd.setColor(0,0.8,0);
		e.addFigura(std::make_shared<Plano>(paredd));
		//  -----------------
		Plano paredFondo(-FRONT, 2.0*distanciaParedes);
		// paredFondo.setTextura(std::make_shared<Textura>(tex));
		// paredFondo.setColor(0.3,0.75,0.9);
		paredFondo.setMaterial(DIFUSO_AZUL);
		e.addFigura(std::make_shared<Plano>(paredFondo));
		// --------------------
		//
		Plano paredOculta(FRONT, distanciaParedes);
		paredOculta.setMaterial(DIFUSO_AZUL);
		// paredOculta.setColor(0.05,0.05,0.8);
		e.addFigura(std::make_shared<Plano>(paredOculta));
		// Figuras:
		//Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 posLuz = centroSuelo + 1.8 * distanciaParedes * UP;
		Color emisionLuces(100);
		LuzPuntual luz(posLuz, emisionLuces);
		e.addLuz(luz);

		// LuzPuntual luz2(posLuz - FRONT * distanciaParedes /2, emisionLuces);
		// e.addLuz(luz2);
		//
		//
		// LuzPuntual luz3(posLuz + FRONT * distanciaParedes /3, emisionLuces);
		// e.addLuz(luz3);



		for (int i = 0; i<1; i++) {
			float tamEsfera =distanciaParedes/3.0*1.2;



			Esfera esf(centroSuelo + 0.5*tamEsfera*UP + 0.45*distanciaParedes*LEFT, tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf.setMaterial(Material(Color(), Color(0.9,0.9,0.9), Color())); //PRUEBA REFLEXION
			//esf.setMaterial(difusoRojo);
			e.addFigura(std::make_shared<Esfera>(esf));
			//
			Vector3 posEsfDcha = centroSuelo + tamEsfera;
			Esfera esfdcha(posEsfDcha, tamEsfera);// 1*1

			esfdcha.setMaterial(DIFUSO_AZUL);
			// esfdcha.setRandomColor();
			//e.addFigura(std::make_shared<Esfera>(esfdcha));
			// double tamEsferaInterior = tamEsfera *0.95;
			// std::cout << "tam:" << tamEsferaInterior << '\n';
			// Esfera esfdchaInterior(posEsfDcha, tamEsferaInterior);// 1*1
			//
			// esfdchaInterior.setMaterial(DIFUSO_VERDE);
			// // esfdcha.setRandomColor();
			// e.addFigura(std::make_shared<Esfera>(esfdchaInterior));
			Esfera esf2(centroSuelo /*+ distanciaParedes*UP*/, 0.75*tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf2.setMaterial(difusoGris);
			esf2.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esf2));
			Vector3 posesfTex=(posEsfDcha + tamEsfera * LEFT);
			// Textura tex=Textura(t,2.0*tamEsfera,2.0*tamEsfera, posesfTex);
			Esfera esfVidrio(posesfTex, tamEsfera, std::make_shared<Textura>(tex));
			// esfLuz.setRandomColor();
			esfVidrio.setMaterial(VIDRIO);
			e.addFigura(std::make_shared<Esfera>(esfVidrio));
			// Vector3 tamPrisma(2.0*tamEsfera, 2.0*tamEsfera,0.2*tamEsfera, false);
			// tamPrisma = tamPrisma/2.0;
			// Prisma caja(centroSuelo + 1.5*distanciaParedes*UP + tamPrisma/2.0 * LEFT, tamPrisma);// 1*1
			// // cout << esf.to_string() << endl;
			// // caja.setMaterial(Material(Color(), Color(0.9,0.9,0.9), Color()));
			// // caja.setColor(0.32,0.2,0.2);
			// caja.setRandomColor();
			// e.addFigura(std::make_shared<Prisma>(caja));

			// Prisma caja2(centroSuelo + 0.5*distanciaParedes*UP + tamPrisma/2.0 * LEFT, tamPrisma);// 1*1
			// // cout << esf.to_string() << endl;
			// caja2.setMaterial(Material(Color(), Color(0.9,0.9,0.9), Color()));
			// caja2.setColor(0.2,0.5,0.2);
			// e.addFigura(std::make_shared<Prisma>(caja2));
			// ESPONJA:
			Vector3 tamEsponja(tamEsfera);
			Vector3 posPrsima = centroSuelo + 1.1*distanciaParedes*LEFT+ 0.5*distanciaParedes*FRONT;
			GeneradorEstructuras gen(GeneradorEstructuras::Estructura::MengerSponge, posPrsima - (2.0*distanciaParedes-tamEsponja[0])*LEFT , tamEsponja, 4);
			auto figuras = gen.getVectorFiguras(); // Devuelve un puntero al vector de las figuras
			for (auto f : *figuras) {
				f->setMaterial(PLASTICO_GRIS);
			}
			// e.addFiguras(figuras);
			PrismaRotable test(posPrsima - (2.0*distanciaParedes-2.0*tamEsponja[0])*LEFT+2.0*tamEsponja[0]*UP-distanciaParedes/2.0*FRONT, tamEsponja/1.5);
			Matriz4 rotacion, tmp;
			rotacion.setRotarZ(PI/4.0);
			tmp.setRotarX(PI/4.0);
			rotacion = tmp * rotacion;
			test.rotar(rotacion);
			std::cout << "rotable: \n" << test.to_string()<<'\n';
			test.setMaterial(PLASTICO_GRIS);
			e.addFigura(std::make_shared<PrismaRotable>(test));

		} // LUZ:
		Vector3 tamPrismaLuz = -LEFT + FRONT + UP / 15.0;
		tamPrismaLuz = tamPrismaLuz*2;
		Vector3 posPrismaLuz = centroSuelo + UP * distanciaParedes * 1.99 + LEFT * tamPrismaLuz[0]/2.0 - FRONT*tamPrismaLuz[2]/2.0;
		Prisma prismaLuz(posPrismaLuz, tamPrismaLuz);
		prismaLuz.setColor(Color(20));
		e.addFigura(std::make_shared<Prisma>(prismaLuz));


		return std::make_unique<Escena>(e);
}


std::unique_ptr<Escena> escenaSimpleDePrueba(const int pixelesX, const int pixelesY, const int rayosPP) {
		// ----------------------- Constantes de la escena:
		double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		Vector3 fCam = FRONT;//(0,1,0,false);
		Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		// int rayosPP =500; // rayos por pixel
		Imagen t= Imagen("textura1.ppm", true);
		Textura tex=Textura(t,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);


		// ----------------------- Camara:
		// Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		// std::cout << gradosARad(90) << '\n'<< PI/4.0 <<'\n';
		double fov = gradosARad(90); //0.475 * PI;
		Camara c = Camara(posCam, centroHabitacion, uCam, fov, pixelesX, pixelesY, rayosPP);


		//

		Escena e(std::make_shared<Camara>(c));//Materiales);//MaterialesSinBVH



		Material difuso(Material::Difuso);
		Material difusoVerde = difuso;
		difusoVerde.setCoeficiente(Color(0,0.8,0),0);
		Material difusoRojo = difuso;
		difusoRojo.setCoeficiente(Color(0.8,0,0),0);
		Material difusoGris = DIFUSO_GRIS;
		//
		// Caja:
		Plano suelo(UP, 1.0*distanciaParedes);
		// suelo.setColor(0.8,0.8,0.8);
		Textura texRotada = tex;
		Matriz4 rotaciontex;
		rotaciontex.setRotarX(gradosARad(90));
		texRotada.rotar(rotaciontex);
		suelo.setTextura(std::make_shared<Textura>(texRotada));
		suelo.setMaterial(difusoGris);
		e.addFigura(std::make_shared<Plano>(suelo));

		Plano techo(-UP, distanciaParedes);
		// techo.setColor(2.5,2.5,2.5);
		techo.setMaterial(DIFUSO_BLANCO);
		e.addFigura(std::make_shared<Plano>(techo));
		Plano paredi(-LEFT, distanciaParedes);
		 // paredi.setColor(0.8,0,0);
		paredi.setMaterial(difusoRojo);
		e.addFigura(std::make_shared<Plano>(paredi));
		Plano paredd(LEFT, distanciaParedes);

		paredd.setMaterial(DIFUSO_ROJO);
		 // paredd.setColor(0,0.8,0);
		e.addFigura(std::make_shared<Plano>(paredd));
		//  -----------------
		Plano paredFondo(-FRONT, 2.0*distanciaParedes);
		// paredFondo.setTextura(std::make_shared<Textura>(tex));
		// paredFondo.setColor(0.3,0.75,0.9);
		paredFondo.setMaterial(DIFUSO_AZUL);
		e.addFigura(std::make_shared<Plano>(paredFondo));
		// --------------------
		//
		Plano paredOculta(FRONT, distanciaParedes);
		paredOculta.setMaterial(DIFUSO_AZUL);
		// paredOculta.setColor(0.05,0.05,0.8);
		e.addFigura(std::make_shared<Plano>(paredOculta));
		// Figuras:
		//Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 posLuz = centroSuelo + 1.8 * distanciaParedes * UP;
		Color emisionLuces(4);
		LuzPuntual luz(posLuz, emisionLuces);
		e.addLuz(luz);

		float tamEsfera = distanciaParedes/0.00060;

		Esfera esfVidrio(centroHabitacion, tamEsfera);
		// esfLuz.setRandomColor();
		esfVidrio.setMaterial(DIFUSO_BLANCO);
		e.addFigura(std::make_shared<Esfera>(esfVidrio));
		std::cout << "escena: " << e << '\n';

		return std::make_unique<Escena>(e);
}



/**************** Programa principal ****************/
int main(int argc, char* argv[]) {
	if (argc < 5) {// <fichero de entrada>
		cerr << "invocar como:\n" << argv[0] << " <fichero de salida> <numero de threads> <rayospp> <resolColor>\n";
		exit(1);
	}
	// escenaBastanteGuay400prismas200esferas(argv);
	// escenaEsponja(argv);
	// escenaPlanos(argv);
	//escenaBastanteGuay400prismas200esferas(argv);
	auto escena = escenaCornellBoxMateriales(300, 300, atoi(argv[3])); // pixX, pixY, rayosPP
	escena->setMaterialFiguras({DIFUSO_ROJO, DIFUSO_AZUL, DIFUSO_BLANCO, DIFUSO_GRIS});
	int nThreads = atoi(argv[2]);
  auto tipo = Renderer::TipoRender::Materiales;//Materiales;//FotonesRadioFijo;//FotonMasCercano;//;FotonesRadioFijo;//Materiales;//VectoresWiReflexion;//Materiales;//VectoresWiRefraccion;krFresnel
	bool usarBVH = true;
	int resColor = atoi(argv[4]); // maxFloat de hdr
	// Renderer rend(*escena, nThreads, tipo, usarBVH);

	// Parametros de PM:
	int maxNumFotones= 100000,
			maxFotonesGlobales= 100000, maxFotonesCausticos= 0,
			nFotonesCercanos = 50;
	bool guardarDirectos = false;
	// Renderer de photon mapping:
	PMRenderer pmrend(*escena, 1, tipo, false, resColor, maxNumFotones,
		maxFotonesGlobales, maxFotonesCausticos, nFotonesCercanos,
		guardarDirectos);
		
	pmrend.render(argv[1]);

	return 0;
}
