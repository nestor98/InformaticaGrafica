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


#include "generador.hpp"

using namespace std;



std::unique_ptr<Escena> escenaCornellBoxMateriales(const int pixelesX, const int pixelesY, const int rayosPP) {

		Vector3 posCam(0,0,0,true);
		Vector3 fCam = FRONT;//(0,1,0,false);
		Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		// int rayosPP =500; // rayos por pixel

		Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);

		//c.setFOV(0.4*PI);
		// c.setFOV(PI);

		//

		Escena e(std::make_shared<Camara>(c));//Materiales);//MaterialesSinBVH

		float distanciaParedes = 3;


		Material difuso(Material::Difuso);
		Material difusoVerde = difuso;
		difusoVerde.setCoeficiente(Color(0,0.8,0),0);
		Material difusoRojo = difuso;
		difusoRojo.setCoeficiente(Color(0.8,0,0),0);
		Material difusoGris = DIFUSO_GRIS;
		//
		// // Caja:
		Plano suelo(UP, 1.0*distanciaParedes);
		// suelo.setColor(0.8,0.8,0.8);
		suelo.setMaterial(difusoGris);
		e.addFigura(std::make_shared<Plano>(suelo));

		Plano techo(-UP, distanciaParedes);
		techo.setColor(1,1,1);
		// techo.setMaterial(difusoGris);
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
		paredFondo.setMaterial(ESPEJO);
		e.addFigura(std::make_shared<Plano>(paredFondo));
		//
		//
		Plano paredOculta(FRONT, 0.1*distanciaParedes);
		paredOculta.setMaterial(DIFUSO_AZUL);
		// paredOculta.setColor(0.05,0.05,0.8);
		e.addFigura(std::make_shared<Plano>(paredOculta));
		// Figuras:
		//Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 centroSuelo = 1.5*distanciaParedes*FRONT - distanciaParedes*UP;
		for (int i = 0; i<1; i++) {
			float tamEsfera =distanciaParedes/3.0*1.2;

			Esfera esf(centroSuelo + tamEsfera*UP + 0.45*distanciaParedes*LEFT, tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf.setMaterial(Material(Color(), Color(0.9,0.9,0.9), Color())); //PRUEBA REFLEXION
			//esf.setMaterial(difusoRojo);
			e.addFigura(std::make_shared<Esfera>(esf));
			//
			Esfera esfdcha(centroSuelo + 0.25*tamEsfera*FRONT+tamEsfera*UP - 0.45*distanciaParedes*LEFT, 1.25*tamEsfera);// 1*1

			esfdcha.setMaterial(VIDRIO);
			// esfdcha.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esfdcha));
			Esfera esf2(centroSuelo /*+ distanciaParedes*UP*/, tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			// esf2.setMaterial(difusoGris);
			esf2.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esf2));
			Vector3 tamPrisma(2.0*tamEsfera, 2.0*tamEsfera,0.2*tamEsfera, false);
			tamPrisma = tamPrisma/2.0;
			Prisma caja(centroSuelo + 1.5*distanciaParedes*UP + tamPrisma/2.0 * LEFT, tamPrisma);// 1*1
			// cout << esf.to_string() << endl;
			// caja.setMaterial(Material(Color(), Color(0.9,0.9,0.9), Color()));
			// caja.setColor(0.32,0.2,0.2);
			caja.setRandomColor();
			e.addFigura(std::make_shared<Prisma>(caja));

			Prisma caja2(centroSuelo + 0.5*distanciaParedes*UP + tamPrisma/2.0 * LEFT, tamPrisma);// 1*1
			// cout << esf.to_string() << endl;
			caja2.setMaterial(Material(Color(), Color(0.9,0.9,0.9), Color()));
			// caja2.setColor(0.2,0.5,0.2);
			// e.addFigura(std::make_shared<Prisma>(caja2));
			// ESPONJA:
			Vector3 tamEsponja(tamEsfera);
			Vector3 posEsponja = centroSuelo + 3.0*tamEsfera*UP + distanciaParedes*LEFT;
			GeneradorEstructuras gen(GeneradorEstructuras::Estructura::MengerSponge, posEsponja, tamEsponja, 4);
			auto figuras = gen.getVectorFiguras(); // Devuelve un puntero al vector de las figuras
			for (auto f : *figuras) {
				f->setMaterial(METAL_GRIS);
			}
			//e.addFiguras(figuras);
			// Prisma test(centroSuelo-LEFT*tamEsfera, tamEsponja);
			// test.setRandomColor();
			// e.addFigura(std::make_shared<Prisma>(test));

		} // LUZ:
		// Vector3 tamPrismaLuz = -LEFT + FRONT + UP / 15.0;
		// tamPrismaLuz = tamPrismaLuz*30.0;
		// Vector3 posPrismaLuz = centroSuelo + UP * distanciaParedes * 1.95 + LEFT * tamPrismaLuz[0]/2.0 - FRONT*tamPrismaLuz[2]/2.0;
		// Prisma luz(posPrismaLuz, tamPrismaLuz);
		// luz.setColor(1,1,1);
		// e.addFigura(std::make_shared<Prisma>(luz));


		return std::make_unique<Escena>(e);
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
	auto escena = escenaCornellBoxMateriales(400, 400, 200); // pixX, pixY, rayosPP
	int nThreads = 12;
  auto tipo = Renderer::TipoRender::Materiales;
	bool usarBVH = true;
	Renderer rend(*escena, nThreads, tipo, usarBVH);
	rend.render(argv[1]);

	// fixIluminacion(argv);
}
