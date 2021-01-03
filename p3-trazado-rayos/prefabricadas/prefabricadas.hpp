#pragma once
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



std::unique_ptr<Escena> escenaCornellBoxMateriales(const int pixelesX, const int pixelesY, const int rayosPP) {
		// ----------------------- Constantes de la escena:
		double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		// Vector3 fCam = FRONT;//(0,1,0,false);
		// Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		// int rayosPP =500; // rayos por pixel
		Imagen t= Imagen("textura1.ppm", true);
		Textura tex=Textura(t,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);


		// ----------------------- Camara:
		// Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		// std::cout << gradosARad(90) << '\n'<< PI/4.0 <<'\n';
		double fov = gradosARad(60); //0.475 * PI;

		Camara c = Camara(posCam-(centroHabitacion-posCam).getModulo()*FRONT,
		centroHabitacion, uCam, fov, pixelesX, pixelesY, rayosPP);


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

		Imagen bump= Imagen("sponza-bumpmap-p3.ppm", true);
		// Textura tex=Textura(t,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);
		Textura texRotada = Textura(bump,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);
		Matriz4 rotaciontex;
		rotaciontex.setRotarX(gradosARad(90));
		texRotada.rotar(rotaciontex);
		texRotada.setMaxDesplaz(5);
		suelo.setMaterial(difusoGris);
		suelo.setBumpMap(std::make_shared<Textura>(texRotada));
		suelo.setTextura(std::make_shared<Textura>(texRotada));
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

		paredd.setMaterial(DIFUSO_VERDE_MAJO);
		std::cout << "DIFUSO_VERDE_MAJO: " << DIFUSO_VERDE_MAJO << std::endl;
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
		Plano paredOculta(FRONT, distanciaParedes*2.5);
		paredOculta.setMaterial(DIFUSO_AZUL);
		// paredOculta.setColor(0.05,0.05,0.8);
		e.addFigura(std::make_shared<Plano>(paredOculta));
		// Figuras:
		//Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 posLuz = centroSuelo + 1.8 * distanciaParedes * UP;
		Color emisionLuces(40);
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

			test.setMaterial(PLASTICO_GRIS);
			e.addFigura(std::make_shared<PrismaRotable>(test));

		} // LUZ:
		Vector3 tamPrismaLuz = -LEFT + FRONT + UP / 15.0;
		tamPrismaLuz = tamPrismaLuz*2;
		Vector3 posPrismaLuz = centroSuelo + UP * distanciaParedes * 1.99 + LEFT * tamPrismaLuz[0]/2.0 - FRONT*tamPrismaLuz[2]/2.0;
		Prisma prismaLuz(posPrismaLuz, tamPrismaLuz);
		prismaLuz.setColor(Color(20));
		//e.addFigura(std::make_shared<Prisma>(prismaLuz));


		return std::make_unique<Escena>(e);
}


std::unique_ptr<Escena> escenaSimpleDePrueba(const int pixelesX, const int pixelesY, const int rayosPP) {
		// ----------------------- Constantes de la escena:
		double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		// Vector3 fCam = FRONT;//(0,1,0,false);
		// Vector3 lCam = LEFT; //(1,0,0,false);
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
		//std::cout << "escena: " << e << '\n';

		return std::make_unique<Escena>(e);
}



std::unique_ptr<Escena> cornellBoxEsferaGrande(const int pixelesX, const int pixelesY, const int rayosPP) {
		// ----------------------- Constantes de la escena:
		double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		// Vector3 fCam = FRONT;//(0,1,0,false);
		// Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		// int rayosPP =500; // rayos por pixel
		Imagen t= Imagen("textura1.ppm", true);
		Textura tex=Textura(t,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);


		// ----------------------- Camara:
		// Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		// std::cout << gradosARad(90) << '\n'<< PI/4.0 <<'\n';
		double fov = gradosARad(60); //0.475 * PI;

		Camara c = Camara(posCam-(centroHabitacion-posCam).getModulo()*FRONT,
		centroHabitacion, uCam, fov, pixelesX, pixelesY, rayosPP);


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

		paredd.setMaterial(DIFUSO_VERDE_MAJO);
		//cout << "DIFUSO_VERDE_MAJO: " << DIFUSO_VERDE_MAJO << endl;
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
		Plano paredOculta(FRONT, distanciaParedes*2.5);
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

      double tamEsferaVidrio = tamEsfera;
      Vector3 posEsferaVidrio = posesfTex + tamEsferaVidrio*UP - 0.5*tamEsferaVidrio*FRONT;
			// Textura tex=Textura(t,2.0*tamEsfera,2.0*tamEsfera, posesfTex);
			Esfera esfVidrio(posEsferaVidrio, tamEsferaVidrio, std::make_shared<Textura>(tex));
			// esfLuz.setRandomColor();
			esfVidrio.setMaterial(VIDRIO);
			e.addFigura(std::make_shared<Esfera>(esfVidrio));
			Vector3 posEsferaVidrioDcha = posEsferaVidrio-1.3*LEFT*tamEsferaVidrio-UP*tamEsferaVidrio;
			Esfera esfVidrioDcha(posEsferaVidrioDcha, tamEsferaVidrio, std::make_shared<Textura>(tex));
			// esfLuz.setRandomColor();
			esfVidrioDcha.setMaterial(VIDRIO);
			e.addFigura(std::make_shared<Esfera>(esfVidrioDcha));


		}

		return std::make_unique<Escena>(e);
}





std::unique_ptr<Escena> cornellBoxArbolMk1(const int pixelesX, const int pixelesY, const int rayosPP) {
		// ----------------------- Constantes de la escena:
		double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		// Vector3 fCam = FRONT;//(0,1,0,false);
		// Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);
		//Camara c(posCam, dirCam);
		//cout << c << endl;
		// int rayosPP =500; // rayos por pixel
		Imagen t= Imagen("textura1.ppm", true);
		Textura tex=Textura(t,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);


		// ----------------------- Camara:
		// Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		// std::cout << gradosARad(90) << '\n'<< PI/4.0 <<'\n';
		double fov = gradosARad(60); //0.475 * PI;
		posCam = posCam-(centroHabitacion-posCam).getModulo()*FRONT;
		Camara c = Camara(posCam, centroHabitacion, uCam, fov, pixelesX,
			 pixelesY, rayosPP);


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

		paredd.setMaterial(DIFUSO_VERDE_MAJO);
		//cout << "DIFUSO_VERDE_MAJO: " << DIFUSO_VERDE_MAJO << endl;
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
		Plano paredOculta(FRONT, distanciaParedes*2.5);
		paredOculta.setMaterial(DIFUSO_AZUL);
		// paredOculta.setColor(0.05,0.05,0.8);
		e.addFigura(std::make_shared<Plano>(paredOculta));
		// Figuras:
		//Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 posLuz = centroSuelo + 1.8 * distanciaParedes * UP;
		Color emisionLuces(100);
		LuzPuntual luz(posLuz, emisionLuces);
		//e.addLuz(luz);

		// LuzPuntual luz2(posLuz - FRONT * distanciaParedes /2, emisionLuces);
		// e.addLuz(luz2);
		//
		//
		// LuzPuntual luz3(posLuz + FRONT * distanciaParedes /3, emisionLuces);
		// e.addLuz(luz3);



		for (int i = 0; i<1; i++) {
			float tamEsfera =distanciaParedes/3.0*1.2;
			Vector3 posEsfDcha = centroSuelo + tamEsfera;

			Esfera esf2(centroSuelo /*+ distanciaParedes*UP*/, 0.75*tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf2.setMaterial(difusoGris);
			esf2.setRandomColor();
			e.addFigura(std::make_shared<Esfera>(esf2));
			Vector3 posesfTex=(posEsfDcha + tamEsfera * LEFT);

      double tamEsferaVidrio = tamEsfera;
      Vector3 posEsferaVidrio = posesfTex + tamEsferaVidrio*UP - 0.5*tamEsferaVidrio*FRONT;
			// Textura tex=Textura(t,2.0*tamEsfera,2.0*tamEsfera, posesfTex);
			Esfera esfVidrio(posEsferaVidrio, tamEsferaVidrio, std::make_shared<Textura>(tex));
			// esfLuz.setRandomColor();
			esfVidrio.setMaterial(VIDRIO);
			e.addFigura(std::make_shared<Esfera>(esfVidrio));

			// Arbol---------------------------------------------
			Vector3 tamRama(tamEsferaVidrio/15.0);
			tamRama[2] = tamRama[2]*7.0;
			Matriz4 baseArbol = BASE_UNIVERSAL;

			// Detras de la camara
			Vector3 posLuz = posCam - 1.5*FRONT;
			Color emisionLuces(200);
			LuzPuntual luz2(posLuz, emisionLuces);
			e.addLuz(luz2);


			baseArbol[3] = posCam - 0.2*FRONT - 1*UP*tamEsferaVidrio; // Detras de la camara
			//baseArbol[3] = posEsferaVidrio-1.3*LEFT*tamEsferaVidrio-1.5*UP*tamEsferaVidrio; // posicion
			baseArbol[3].setPunto(); // Tiene que ser un pto
			GeneradorEstructuras gen(GeneradorEstructuras::Estructura::ArbolPrismas,
				 baseArbol, tamRama, 4);
			auto figuras = gen.getVectorFiguras(); // Devuelve un puntero al vector de las figuras
			// for (auto f : *figuras) {
			// 	//f->setMaterial(PLASTICO_GRIS);
			// }
			e.addFiguras(figuras);
		}

		return std::make_unique<Escena>(e);
}
