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
// #include "renderer.hpp"
// #include "pm-renderer.hpp"


// Escenas adaptadas a partir de las del main proporcionado de PM
std::unique_ptr<Escena> escenaPruebas(const int pixelesX, const int pixelesY, const int rayosPP, const int escena) {
						// // Vector3 camera_position(0,1,3.0);
						// // Vector3 looking_at(0,1,0);
						// // Vector3 up(0,1,0);
						// // Real view_plane_dist = 1;
						// // Camera camera(camera_position,looking_at,up,view_plane_dist);


						// Vector3 looking_at(0.0,1.0,0, true);
						// Vector3 posicion_camara(0,1.0,3.0, true);

						// double distancia=1;
						// Camara c= Camara(posicion_camara, looking_at, FRONT, gradosARad(90), 512, 512, 20);

						// Escena e(std::make_shared<Camara>(c));

						// // BSDF* glass = new Transmissive(w, 1.5);
						// // BSDF* mirror = new Specular(w);

						// // BSDF* white = new Lambertian(w, Vector3(.85,.85,.85));
						// // BSDF* red = new Lambertian(w, Vector3(.85,.085,.085));
						// // BSDF* green = new Lambertian(w, Vector3(.085,.85,.085));
								double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		// Vector3 fCam = FRONT;//(0,1,0,false);
		// Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);


		// ----------------------- Camara:
		// Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		// std::cout << gradosARad(90) << '\n'<< PI/4.0 <<'\n';
		double fov = gradosARad(60); //0.475 * PI;

		Camara c = Camara(posCam-(centroHabitacion-posCam).getModulo()*FRONT,
		centroHabitacion, uCam, fov, pixelesX, pixelesY, rayosPP);


		//

		Escena e(std::make_shared<Camara>(c));

	Material cristal = Material(Color(), Color(), Color(0.9,0.9,0.9), 1.5f);
	Material espejo = Material(Color(), Color(0.9,0.9,0.9), Color());
	Material difusoBlanco(Color(.85,.85,.85), Color(), Color());
	Material difusoRojo(Color(.85,.085,.085), Color(), Color());
	Material difusoVerde(Color(.085,.85,.085), Color(), Color());

						// // Plano suelo(UP, distancia);
						// // suelo.setMaterial(difusoBlanco);
						// // e.addFigura(std::make_shared<Plano>(suelo));


						// // Plano techo(-UP, distancia);
						// // techo.setMaterial(difusoBlanco);
						// // e.addFigura(std::make_shared<Plano>(techo));



						// 	// Plano paredFondo(-FRONT, distancia);
						// 	// paredFondo.setMaterial(difusoBlanco);
						// 	// e.addFigura(std::make_shared<Plano>(paredFondo));


						// 	Plano paredi(-LEFT, distancia);
						// 	paredi.setMaterial(difusoRojo);
						// 	e.addFigura(std::make_shared<Plano>(paredi));


						// 	Plano paredd(LEFT, distancia);
						// 	paredd.setMaterial(difusoVerde);
						// 	e.addFigura(std::make_shared<Plano>(paredd));

		Plano suelo(UP, 1.0*distanciaParedes);

		suelo.setMaterial(difusoBlanco);
		e.addFigura(std::make_shared<Plano>(suelo));

		Plano techo(-UP, distanciaParedes);
		// techo.setColor(2.5,2.5,2.5);
		techo.setMaterial(difusoBlanco);
		e.addFigura(std::make_shared<Plano>(techo));
		Plano paredi(-LEFT, distanciaParedes);
		 // paredi.setColor(0.8,0,0);
		paredi.setMaterial(difusoRojo);
		e.addFigura(std::make_shared<Plano>(paredi));
		Plano paredd(LEFT, distanciaParedes);

		paredd.setMaterial(difusoVerde);

		e.addFigura(std::make_shared<Plano>(paredd));
		//  -----------------
		Plano paredFondo(-FRONT, 2.0*distanciaParedes);
		paredFondo.setMaterial(difusoBlanco);
		e.addFigura(std::make_shared<Plano>(paredFondo));

		float tamEsfera=0.8;
		Vector3 pos1=centroSuelo+UP*tamEsfera+LEFT*tamEsfera-FRONT*tamEsfera;
		Vector3 pos2=centroSuelo+UP*tamEsfera-2*LEFT*tamEsfera;

	switch(escena){
		case 1:
		{
		// 	Object3D* sphere1 = new Sphere(Vector3(0.5,0.3,.5), 0.3, glass);
		// w->add_object(sphere1);

		// Object3D* sphere2 = new Sphere(Vector3(-0.5,0.5,.5), 0.3, mirror);
		// w->add_object(sphere2);
			Esfera esf(pos1, tamEsfera);
			esf.setMaterial(cristal);
			e.addFigura(std::make_shared<Esfera>(esf));
			Esfera esf2(pos2, tamEsfera);
			esf2.setMaterial(espejo);
			e.addFigura(std::make_shared<Esfera>(esf2));

		}
		break;
		case 2:
		{

		// Object3D* sphere1 = new Sphere(Vector3(0.5,0.3,.5), 0.3, white);
		// w->add_object(sphere1);

		// Object3D* sphere2 = new Sphere(Vector3(-0.5,0.5,1.5), 0.3, red);
		// w->add_object(sphere2);

		// Object3D* sphere3 = new Sphere(Vector3(0.,0.3,.0), 0.3, white);
		// w->add_object(sphere3);
			Esfera esf(pos1+1.2*LEFT, tamEsfera);
			esf.setMaterial(difusoBlanco);
			e.addFigura(std::make_shared<Esfera>(esf));
			Esfera esf2(pos2-FRONT, tamEsfera);
			esf2.setMaterial(difusoRojo);
			e.addFigura(std::make_shared<Esfera>(esf2));
			Esfera esf3(pos2+2*LEFT+FRONT/2, tamEsfera);
			esf3.setMaterial(difusoBlanco);
			e.addFigura(std::make_shared<Esfera>(esf3));

		}
		break;
		case 4:
		{
		// Object3D* sphere1 = new Sphere(Vector3(0.5,0.3,.5), 0.3, white);
		// w->add_object(sphere1);

		// Object3D* sphere2 = new Sphere(Vector3(-0.5,0.5,1.5), 0.3, red);
		// w->add_object(sphere2);
			float tamEsfera=0.3;
			Esfera esf(Vector3(0.5,0.3,.5, true), tamEsfera);
			esf.setMaterial(difusoBlanco);
			e.addFigura(std::make_shared<Esfera>(esf));
			Esfera esf2(Vector3(-0.5,0.5,1.5, true), tamEsfera);
			esf2.setMaterial(difusoRojo);
			e.addFigura(std::make_shared<Esfera>(esf2));
		}
		break;

		default:
		break;

	}

		Color emisionLuces(20);//40 //8
		LuzPuntual luz(Vector3(0,1.9,0, true), emisionLuces);
		e.addLuz(luz);
		// LightSource* ls = new PointLightSource(w, Vector3(0,1.9,0), Vector3(5,5,5));
		// w->add_light(ls);


	return std::make_unique<Escena>(e);


}


// Mar y reflejo del sol la luna o algo
std::unique_ptr<Escena> escenaDosPlanos(const int pixelesX, const int pixelesY, const int rayosPP) {

								double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - UP * distanciaParedes/4.0;
		// Vector3 fCam = FRONT;//(0,1,0,false);
		// Vector3 lCam = LEFT; //(1,0,0,false);
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);


		// ----------------------- Camara:
		// Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		// std::cout << gradosARad(90) << '\n'<< PI/4.0 <<'\n';
		double fov = gradosARad(60); //0.475 * PI;

		Camara c = Camara(posCam-(centroHabitacion-posCam).getModulo()*FRONT,
		centroHabitacion, uCam, fov, pixelesX, pixelesY, rayosPP);


		//

		Escena e(std::make_shared<Camara>(c));

	Material cristal = Material(Color(), Color(), Color(0.9,0.9,0.9), 1.5f);
	Material espejo = Material(Color(), Color(0.6,0.6,0.6), Color());
	Material difusoBlanco(Color(.85,.85,.85), Color(), Color());
	Material difusoRojo(Color(.85,.085,.085), Color(), Color());
	Material difusoVerde(Color(.085,.85,.085), Color(), Color());

		Plano suelo(UP, 1.0*distanciaParedes);
		Imagen bump= Imagen("bump-agua-p3.ppm", true);// bump-agua de https://forums.chaosgroup.com/forum/chaos-common/chaos-common-public/1008483-v-ray-3-6-ocean-water-again
		Textura texRotada = Textura(bump,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);
		Matriz4 rotaciontex;
		rotaciontex.setRotarX(gradosARad(90));
		texRotada.rotar(rotaciontex);
		texRotada.setMaxDesplaz(3e-2);
		suelo.setMaterial(ESPEJO);
		suelo.setBumpMap(std::make_shared<Textura>(texRotada));
		e.addFigura(std::make_shared<Plano>(suelo));

		Plano techo(-UP, 5000);
		//techo.setColor(5*Color(135/255.0,206/255.0,235/255.0));
		techo.setColor(Color());

		//techo.setMaterial(difusoBlanco);
	//	e.addFigura(std::make_shared<Plano>(techo));

		float tamEsfera=100;
		Vector3 pos1=centroSuelo+FRONT*1000;
		Vector3 pos2=pos1+UP*300;


		// Esfera esf(pos2, tamEsfera);
		// esf.setColor(30*Color(252/255.0, 212/255.0, 64/255.0));
		// e.addFigura(std::make_shared<Esfera>(esf));

		double dmin = 2.0*tamEsfera, dmax = 2.0*tamEsfera,// distanciaParedes,
					rmin = tamEsfera/10, rmax=tamEsfera/10;
		int nEsferas = 200;
		GeneradorEstructuras gen(GeneradorEstructuras::Estructura::ExplosionEsferas, pos2,
			dmin, dmax, rmin, rmax, nEsferas);
		auto figuras = gen.getVectorFiguras(); // Devuelve un puntero al vector de las figuras
		e.addFiguras(figuras);

	return std::make_unique<Escena>(e);


}




std::unique_ptr<Escena> escenaPruebaSky(const int pixelesX, const int pixelesY, const int rayosPP) {
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
		GeneradorEstructuras gen=GeneradorEstructuras(GeneradorEstructuras::Estructura::SkyBox, "texturas/",6.0, 8);
		auto figuras = gen.getVectorFiguras(); // Devuelve un puntero al vector de las figuras
		e.addFiguras(figuras);

		// Figuras:
		//Esfera esf(posEsf+5.0*(0.3*i*uCam), 0.5);// 1*1
		Vector3 posLuz = centroSuelo + 1.8 * distanciaParedes * UP;
		Color emisionLuces(30);//40 //8
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
			//e.addFiguras(figuras);
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
		double fov = gradosARad(80); //0.475 * PI;

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
		// sponza-bumpmap del pbrt, capitulo bump mapping
		//Imagen bump= Imagen("sponza-bumpmap-p3.ppm", true); // textura-bump.ppm
		//Imagen bump= Imagen("bump-agua-p3.ppm", true);// bump-agua de https://forums.chaosgroup.com/forum/chaos-common/chaos-common-public/1008483-v-ray-3-6-ocean-water-again
		// Textura tex=Textura(t,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);
		// Textura texRotada = Textura(bump,2.0*distanciaParedes/2.0,2.0*distanciaParedes/2.0, 2.0*distanciaParedes+FRONT);
		// Matriz4 rotaciontex;
		// rotaciontex.setRotarX(gradosARad(90));
		// texRotada.rotar(rotaciontex);
		// texRotada.setMaxDesplaz(3e-2);
		// suelo.setMaterial(ESPEJO);
		// suelo.setBumpMap(std::make_shared<Textura>(texRotada));
		//suelo.setTextura(std::make_shared<Textura>(texRotada));
		suelo.setMaterial(DIFUSO_BLANCO);
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
		std::cout << "DIFUSO_VERDE_MAJO: " << DIFUSO_VERDE_MAJO << std::endl;
		 // paredd.setColor(0,0.8,0);
		e.addFigura(std::make_shared<Plano>(paredd));
		//  -----------------
		Plano paredFondo(-FRONT, 2.0*distanciaParedes);
		paredFondo.setTextura(std::make_shared<Textura>(tex));
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
		Color emisionLuces(30);//40 //8
		LuzPuntual luz(posLuz, emisionLuces);
		e.addLuz(luz);

		// LuzPuntual luz2(posLuz - FRONT * distanciaParedes /2, emisionLuces);
		// e.addLuz(luz2);
		//
		//
		// LuzPuntual luz3(posLuz + FRONT * distanciaParedes /3, emisionLuces);
		// e.addLuz(luz3);



		for (int i = 0; i<1; i++) {
			float tamEsfera =distanciaParedes/3.0;



			Esfera esf(centroSuelo + tamEsfera*UP + 0.45*distanciaParedes*LEFT - 0.2*FRONT, tamEsfera);// 1*1
			// cout << esf.to_string() << endl;
			esf.setMaterial(Material(Color(0.3,0.3,0.3), Color(0.9,0.9,0.9), Color())); //PRUEBA REFLEXION
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
			// Esfera esf2(centroSuelo /*+ distanciaParedes*UP*/, 0.75*tamEsfera);// 1*1
			// // cout << esf.to_string() << endl;
			// esf2.setMaterial(difusoGris);
			// esf2.setRandomColor();
			// e.addFigura(std::make_shared<Esfera>(esf2));
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
			e.addFiguras(figuras);
			PrismaRotable test(posPrsima - (2.0*distanciaParedes-2.0*tamEsponja[0])*LEFT+2.0*tamEsponja[0]*UP-distanciaParedes/2.0*FRONT, tamEsponja);
			Matriz4 rotacion, tmp;
			rotacion.setRotarZ(PI/4.0);
			tmp.setRotarX(PI/4.0);
			rotacion = tmp * rotacion;
			test.rotar(rotacion);

			test.setMaterial(PLASTICO_GRIS);
			//e.addFigura(std::make_shared<PrismaRotable>(test));

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




std::unique_ptr<Escena> escenaExplosionEsferas(const int pixelesX, const int pixelesY, const int rayosPP) {
		// ----------------------- Constantes de la escena:
		double distanciaParedes = 3;
		Vector3 centroSuelo =distanciaParedes*FRONT - distanciaParedes*UP;
		Vector3 centroHabitacion = centroSuelo + distanciaParedes * UP;
		Vector3 posCam(0,0,0,true);
		posCam = posCam - FRONT*11;//  UP * distanciaParedes/4.0 -
		Vector3 uCam = UP * double(pixelesY)/double(pixelesX);//(0,0,double(pixelesY)/double(pixelesX),false);

		// ----------------------- Camara:
		// Camara c = Camara(posCam, fCam, lCam, uCam,pixelesX,pixelesY,rayosPP);
		// std::cout << gradosARad(90) << '\n'<< PI/4.0 <<'\n';
		double fov = gradosARad(60); //0.475 * PI;

		Camara c = Camara(posCam-(centroHabitacion-posCam).getModulo()*FRONT,
		centroHabitacion, uCam, fov, pixelesX, pixelesY, rayosPP);
		Escena e(std::make_shared<Camara>(c));//Materiales);//MaterialesSinBVH



		double dmin = 6, dmax = 8,// distanciaParedes,
					rmin = 0.2, rmax=0.2;
		int nEsferas = 2500;
		GeneradorEstructuras gen(GeneradorEstructuras::Estructura::ExplosionEsferas, centroHabitacion,
			dmin, dmax, rmin, rmax, nEsferas,0.0);
		auto figuras = gen.getVectorFiguras(); // Devuelve un puntero al vector de las figuras
		// for (auto f : *figuras) {
		// 	f->setMaterial(PLASTICO_GRIS);
		// }

		// Luz
		Vector3 posLuz = centroHabitacion +  dmax*20.0*(UP-FRONT);
		Color emisionLuces(500);
		LuzPuntual luz(posLuz, emisionLuces);
		e.addLuz(luz);

		e.addFiguras(figuras);
		std::cout << "escena:\n" <<e << '\n';
		return std::make_unique<Escena>(e);
}
