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
#include "progressive-pm-renderer.hpp"

#include "generador.hpp"

#include "prefabricadas.hpp"
using namespace std;


/*
auto escena = escenaCornellBoxMateriales(300, 300, atoi(argv[3])); // pixX, pixY, rayosPP
escena->setMaterialFiguras({DIFUSO_ROJO, DIFUSO_AZUL, DIFUSO_BLANCO});
int nThreads = atoi(argv[2]);
auto tipo = Renderer::TipoRender::Materiales;//Materiales;//FotonesRadioFijo;//FotonMasCercano;//;FotonesRadioFijo;//Materiales;//VectoresWiReflexion;//Materiales;//VectoresWiRefraccion;krFresnel
bool usarBVH = true;
int resColor = atoi(argv[4]); // maxFloat de hdr
// Renderer rend(*escena, nThreads, tipo, usarBVH);

// Parametros de PM:
int maxNumFotones= 100000,
		maxFotonesGlobales= 100000, maxFotonesCausticos= 0,
		nFotonesCercanos = 50;
bool guardarDirectos = true;
// Renderer de photon mapping:
PMRenderer pmrend(*escena, 1, tipo, false, resColor, maxNumFotones,
	maxFotonesGlobales, maxFotonesCausticos, nFotonesCercanos,
	guardarDirectos);

pmrend.render(argv[1]);
*/




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
	/* Path tracer
	auto escena = escenaCornellBoxMateriales(300, 300, atoi(argv[3])); // pixX, pixY, rayosPP
	//escena->setMaterialFiguras({DIFUSO_ROJO, DIFUSO_AZUL, DIFUSO_BLANCO});
	int nThreads = atoi(argv[2]);
  auto tipo = Renderer::TipoRender::Materiales;//Materiales;//FotonesRadioFijo;//FotonMasCercano;//;FotonesRadioFijo;//Materiales;//VectoresWiReflexion;//Materiales;//VectoresWiRefraccion;krFresnel
	bool usarBVH = true;
	int resColor = atoi(argv[4]); // maxFloat de hdr
	Renderer rend(*escena, nThreads, tipo, usarBVH);

	rend.render(argv[1]);*/

	auto escena = escenaCornellBoxMateriales(300, 300, atoi(argv[3])); // pixX, pixY, rayosPP
	//escena->setMaterialFiguras({DIFUSO_ROJO, DIFUSO_AZUL, DIFUSO_BLANCO});
	int nThreads = atoi(argv[2]);
	auto tipo = PMRenderer::TipoRender::Materiales;//Materiales;//FotonesRadioFijo;//FotonMasCercano;//;FotonesRadioFijo;//Materiales;//VectoresWiReflexion;//Materiales;//VectoresWiRefraccion;krFresnel
	bool usarBVH = true;
	int resColor = atoi(argv[4]); // maxFloat de hdr
	// -----------------------------------------------
	// Con Path tracer:
	// Renderer rend(*escena, nThreads, tipo, usarBVH);
	// rend.render(argv[1]);
	// -----------------------------------------------


	// -----------------------------------------------
	// Con PM;
	// Parametros de PM:
	int maxNumFotones= 50000,
			maxFotonesGlobales= 50000, maxFotonesCausticos= 10000,
			nFotonesCercanos = 250;
	bool guardarDirectos = false;
	// Renderer de photon mapping:
	ProgressivePMRenderer pmrend(*escena, nThreads, tipo, usarBVH, resColor, maxNumFotones,
		maxFotonesGlobales, maxFotonesCausticos, nFotonesCercanos,
		guardarDirectos);
	pmrend.iterarRenderAll(30,argv[1]);

	// pmrend.render(argv[1]);
	return 0;
}
