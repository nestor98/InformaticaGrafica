#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>


#include "Imagen.hpp"
#include "camara.hpp"
#include "figura.hpp"
#include "material.hpp"
#include "bvh.hpp"

#include "utils.hpp"

class Escena {
	mutable int numerodePathsdedebugAcuerdateDeBorrarme;// = 0;
	std::shared_ptr<Camara> c;
	std::vector<std::shared_ptr<Figura>> figuras;

	GeneradorAleatorio gen;

	BoundingVolumeH bvh;

	// Auxiliar de render
	void renderPixel(Imagen& im, const Vector3& o, const int pixel) const;

	// Ray tracer, normales, etc
	void renderPixelViejo(Imagen& im, const Vector3& o, const int pixel) const;


	// --------- Threads ---------
	std::vector<std::thread> threads; // Vector con cada thread
	std::mutex mtx; // mutex para asegurar seccion critica (tomar dato de la cola)

	std::vector<int> tasks; // cola de pixeles a renderizar

	// Inicializa los threads:
	void initThreads(Imagen& im, const Vector3& origen);

	void waitThreads();

	// encola un pixel
	void enQueueTask(const int pixel);

	// funcion que ejecutan los threads
	void consumirTasks(Imagen& im, const Vector3& origen);
	// ---------------------------
	// --------- TESTS  ---------
	enum Metodo {Original, BVH};

	void testRenderMethod(const Metodo metodo, const std::string fichero) const;

	void renderPixelVector(Imagen& im, const Vector3& o, const int pixel) const;

	// ---------------------------------------
	// --------- Aux de path tracer  ---------
	Color ruletaRusa(const std::shared_ptr<Figura> fig, const Vector3& pto,int nRebotes, const bool primerRebote=false) const;
	Color pathTrace(const Vector3& o, const Vector3& dir,int nRebotes, const bool primerRebote=false) const;



public:
	enum TipoRender {Emision, Distancia, Normales, Materiales, MaterialesSinBVH};
private:
	TipoRender renderSeleccionado;
public:
	// Escena(const Camara& _c, const TipoRender tipo = BVHEmision);
	Escena(const std::shared_ptr<Camara> _c, const int _nThreads = 16, const TipoRender tipo = Emision);

	std::string to_string() const;
	void addFigura(const std::shared_ptr<Figura> f);

	void addFiguras(const std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectFiguras);

	void render(const std::string fichero);

	// Compara los tiempos de render (secuencial, sin threads) de la escena con
	// el metodo original y el bvh. Muestra los resultados por salida estandar.
	void testBVHRender(const std::string f1 = "out/testRenderOriginal.ppm", const std::string f2 = "out/testRenderBVH.ppm");
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Escena& e);
