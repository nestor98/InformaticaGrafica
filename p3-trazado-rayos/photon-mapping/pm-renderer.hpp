#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
//
// #include "escena.hpp"
// #include "Imagen.hpp"
// #include "camara.hpp"
// #include "figura.hpp"
// #include "material.hpp"
// #include "bvh.hpp"
//
// #include "utils.hpp"
#include "renderer.hpp"
#include "Foton.hpp"
#include "KDTree.h"

const unsigned int MAX_FOTONES = 100000;

class PMRenderer : public Renderer {
	int maxNumFotones, maxFotonesGlobales, maxFotonesCausticos;
	int fotonesActuales;
	KDTree<Foton, MAX_FOTONES> kdTreeFotones;
//
// protected:
// 	bool usarBVH;
// 	BoundingVolumeH bvh;
//
// 	Escena e;

	// Auxiliar de render
	void renderPixel(Imagen& im, const Vector3& o, const int pixel) const;


	// --------- Threads ---------
	std::vector<std::thread> threads; // Vector con cada thread
	std::mutex mtx; // mutex para asegurar seccion critica (tomar dato de la cola)

	std::vector<int> tasks; // cola de pixeles a renderizar

	// Inicializa los threads:
	void initThreads(Imagen& im,  const Vector3& origen, const int nPixeles);

	void waitThreads();

	// encola un pixel
	void enQueueTask(const int pixel);

	// funcion que ejecutan los threads
	void consumirTasks(Imagen& im,  const Vector3& origen);

	// ---------------------------------------
	// --------- Aux de path tracer  --------- // TODO: Se quedan??
	Color ruletaRusa(const std::shared_ptr<Figura> fig, const Vector3& dir, const Vector3& pto, const GeneradorAleatorio& rngThread, const bool primerRebote=false) const;
	Color pathTrace(const Vector3& o, const Vector3& dir, const GeneradorAleatorio& rngThread, const bool primerRebote = false) const;
	Color shadowRay(const Vector3& pto, const int indiceluz) const;

	// ---------------------------------------
	// --------- Barra de progreso  ---------
	void progressBar(const int nPixeles);

	// ---------------------------------------
	// --------- Photon mapping...  ---------
	bool trace_ray(const Escena& e, const Vector3& origen, const Vector3& dir, const Color &p,
  			   std::list<Foton> &fotonesGlobales, std::list<Foton> &fotonesCausticos, bool directo,
 			 	const GeneradorAleatorio& rng);

	void preprocess(const Escena& e);

	Color shade(const Figura::InterseccionData& interseccion,
	  const std::shared_ptr<Figura>& figIntersectada) const;

public:
	enum TipoRender {Materiales, Distancia, Normales, VectoresWiDifusos, VectoresWiReflexion, VectoresWiRefraccion, krFresnel};
protected:
	TipoRender renderSeleccionado;


public:
	// PMRenderer(const Camara& _c, const TipoRender tipo = BVHEmision);
	PMRenderer(const int _nThreads = 12, const TipoRender tipo = Materiales, const bool _usarBVH = true);
	PMRenderer(const Escena& _e, const int _nThreads = 12, const TipoRender tipo = Materiales, const bool _usarBVH = true);

	std::string to_string() const;

	void render(const std::string fichero);
	//

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const PMRenderer& e);

	// ---------------------------
	// --------- TESTS  ---------

		// Compara los tiempos de render (secuencial, sin threads) de la PMRenderer con
		// el metodo original y el bvh. Muestra los resultados por salida estandar.
		// void testBVHRender(const std::string f1 = "out/testRenderOriginal.ppm", const std::string f2 = "out/testRenderBVH.ppm");

	// enum Metodo {Original, BVH};
	//
	// void testRenderMethod(const Metodo metodo, const std::string fichero) const;
	//
	// void renderPixelVector(Imagen& im, const Vector3& o, const int pixel) const;
