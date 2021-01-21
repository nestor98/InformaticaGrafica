#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

#include "renderer.hpp"
#include "Foton.hpp"
#include "KDTree.h"

// const unsigned int MAX_FOTONES = 100000;

class PMRenderer : public Renderer {
protected:
	int maxNumFotones, maxFotonesGlobales, maxFotonesCausticos;
	int fotonesActuales;
	int nFotonesCercanos;
	bool guardarDirectos;
	float radioCausticos;
	KDTree<Foton, 3> kdTreeGlobal, kdTreeCaustico;
	// Auxiliar de render
	void renderPixel(Imagen& im, const Vector3& o, const int pixel,
	const GeneradorAleatorio& rng) const;


	// --------- Threads ---------
	// Inicializa los threads:
	void initThreads(Imagen& im,  const Vector3& origen, const int nPixeles);


	// funcion que ejecutan los threads
	void consumirTasks(Imagen& im,  const Vector3& origen);

	// ---------------------------------------
	// --------- Aux de path tracer  --------- // TODO: Se quedan??
	Color ruletaRusa(const std::shared_ptr<Figura> fig, const Vector3& dir,
		const Vector3& pto, const GeneradorAleatorio& rngThread, const bool primerRebote=false) const;
	Color pathTrace(const Vector3& o, const Vector3& dir, const GeneradorAleatorio& rngThread,
		const bool primerRebote = false) const;



	// ---------------------------------------
	// --------- Photon mapping...  ---------
	bool trace_ray(const Vector3& origen, const Vector3& dir, const Color &p,
			   std::list<Foton> &fotonesGlobales, std::list<Foton> &fotonesCausticos,
				 bool directo, const GeneradorAleatorio& rng);

	void preprocess(bool normalizar=true);

	// Auxiliar de shade para separar la parte de PM del debug
	Color shadePM(const Figura::InterseccionData& interseccion,
  	const std::shared_ptr<Figura>& figIntersectada, const bool primerRebote,
	  const GeneradorAleatorio& rng, const Vector3& dir) const;

	// Aux de iluminacionGlobal y de causticas
	Color iluminacionRadioFijo(const KDTree<Foton,3>& kdTree,
	  const Vector3& ptoInterseccion, const Vector3& normal,
	  const float radio, int& nFotonesCercanos) const;
	// Color iluminacionDeKDTree(const int idxKDTree,
	//   const Vector3& ptoInterseccion,
	//   const Vector3& normal) const;
	// Aux de shadePM, devuelve la luminosidad correspondiente a fotones globales
	Color iluminacionGlobal(const Vector3& pto,
		const Vector3& normal) const;

	// Aux de shadePM, devuelve la luminosidad correspondiente a fotones causticos
	Color causticas(const Vector3& pto,
	  const Vector3& normal) const;

	Color shade(const Figura::InterseccionData& interseccion,
  	const std::shared_ptr<Figura>& figIntersectada, const bool primerRebote,
	  const GeneradorAleatorio& rng, const Vector3& dir) const;

public:
	// PMRenderer(const Camara& _c, const TipoRender tipo = BVHEmision);
	// PMRenderer(const int _nThreads = 12, const TipoRender tipo = Materiales, const bool _usarBVH = true);
	PMRenderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo,
		const bool _usarBVH, const float _rangoDinamico=18, const int _maxNumFotones= 100000,
	  const int _maxFotonesGlobales= 100000, const int _maxFotonesCausticos= 0,
		const int _nFotonesCercanos = 50, const bool _guardarDirectos = true, const float radioCaus=0.05);

	// PMRenderer(const Escena& _e, const int _nThreads = 12, const TipoRender tipo = Materiales, const bool _usarBVH = true);

	std::string to_string() const;

	void render(const std::string fichero);

	// ---------------------- PROGRESSIVE PM -------------------------
	// Render progressive PM, con <iteraciones> y radio inicial r1:
	void render(const std::string fichero, const int iteraciones, const float& r1=0.5);

protected:
	// --------- Threads ---------
	// Inicializa los threads:
	void initThreadsProgressive(Imagen& im,  const Vector3& origen, const int nPixeles, const float& radio);


	// funcion que ejecutan los threads
	void consumirTasksProgressive(Imagen& im,  const Vector3& origen, const float& radio);

	// Auxiliar de render
	void addToPixelProgressive(Imagen& im, const Vector3& o, const int pixel,
	const GeneradorAleatorio& rng, const float& radio) const;

	// Auxiliar de shade para separar la parte de PM del debug
	Color shadeProgressive(const Figura::InterseccionData& interseccion,
  	const std::shared_ptr<Figura>& figIntersectada, const bool primerRebote,
	  const GeneradorAleatorio& rng, const Vector3& dir, const float& radio) const;

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
