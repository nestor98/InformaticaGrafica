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
// #include "progressive-pm-renderer.hpp"

#include "generador.hpp"

#include "prefabricadas.hpp"
using namespace std;

/**************** Programa principal ****************/
int main(int argc, char* argv[]) {
	if (argc < 6) {// <fichero de entrada>
		cerr << "invocar como:\n" << argv[0]
		<< " <fichero de salida> <numero de threads> <rayospp> <resolColor> <0, 1 o 2 (PT,PM,PPM)>\n";
		exit(1);
	}
	// escenaBastanteGuay400prismas200esferas(argv);
	// escenaEsponja(argv);
	// escenaPlanos(argv);
	//escenaBastanteGuay400prismas200esferas(argv);
	/* Path tracer
	auto escena = escenaCornellBoxMateriales(300, 300, atoi(argv[3])); // pixX, pixY, rayosPP
	//escena->setMaterialFiguras({DIFUSO_ROJO, DIFUSO_AZUL, DIFUSO_BLANCO});
	int nThreads = atoi(argv[2]);
  auto tipo = Renderer::TipoRender::Materiales;//Materiales;//FotonesRadioFijo;//FotonMasCercano;//;FotonesRadioFijo;//Materiales;//VectoresWiReflexion;//Materiales;//VectoresWiRefraccion;krFresnel
	bool usarBVH = true;
	int resColor = atoi(argv[4]); // maxFloat de hdr
	Renderer rend(*escena, nThreads, tipo, usarBVH);

	rend.render(argv[1]);*/
	//720/16*9
	auto escena = escenaAguaEsfera(1002, 1002*9/16 , atoi(argv[3])); // pixX, pixY, rayosPP
	//escena->setMaterialFiguras({DIFUSO_ROJO, DIFUSO_AZUL, DIFUSO_BLANCO});
	int nThreads = atoi(argv[2]);
	auto tipo = Renderer::TipoRender::Materiales;//;Normales//;//Materiales;//FotonMasCercano;//;FotonesRadioFijo;//Materiales;//VectoresWiReflexion;//Materiales;//VectoresWiRefraccion;krFresnel
	bool usarBVH = true;
	int resColor = atoi(argv[4]); // maxFloat de hdr

	enum TipoRenderer {PathTracer, PM, ProgressivePM};
	TipoRenderer seleccionado=(TipoRenderer) atoi(argv[5]);

	std::cout << "------- ESCENA -------\n" << escena->to_string() << '\n';
	if (seleccionado==PathTracer){
		// -----------------------------------------------
		// Con Path tracer:
		Renderer rend(*escena, nThreads, tipo, usarBVH, resColor);
		rend.render(argv[1]);
		// -----------------------------------------------
	} else {
		// -----------------------------------------------
		// Con PROGRESSIVE PM;
		// Parametros de PM:
		int maxNumFotones= 100000,
				maxFotonesGlobales= 100000, maxFotonesCausticos= 100000,
				nFotonesCercanos = 100;
		bool guardarDirectos =false;
		float radioCausticos = 1.0f;
		// Parametros PROGRESSIVE:
		int nIteraciones = 10;
		float r1 = 1.0;
		// // Renderer de photon mapping:
		PMRenderer pmrend(*escena, nThreads, tipo, usarBVH, resColor, maxNumFotones,
			maxFotonesGlobales, maxFotonesCausticos, nFotonesCercanos,
			guardarDirectos, radioCausticos);
		// pmrend.iterarRenderAll(15,argv[1]);
		if (seleccionado==PM) {
			pmrend.render(argv[1]);
		} else {
			pmrend.render(argv[1], nIteraciones, r1);
		}
	}

	return 0;
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
