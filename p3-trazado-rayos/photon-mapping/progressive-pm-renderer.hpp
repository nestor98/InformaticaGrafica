/**************************************************************
 * Progressive photon mapping
 *
 *
 **************************************************************/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include "pm-renderer.hpp"


// const unsigned int MAX_FOTONES = 100000;

// Fuente del algoritmo: https://www.ci.i.u-tokyo.ac.jp/~hachisuka/ppm.pdf
class ProgressivePMRenderer : public PMRenderer {
	// Struct con la informacion de un hit para ppm
	// Basado tb en: https://www.ci.i.u-tokyo.ac.jp/~hachisuka/ppm.pdf
	struct Hit {
		Vector3 pto; // posicion
		Vector3 normal;
		Vector3 dir;
		Color kd; // TODO: ??? en el paper era int BRDF (indice del evento {difuso, especular, transmision})
		int iPixel; // Indice del pixel (orden i-d, arriba abajo)
		Color peso; // Pixel weight (????????????)
		float r; // Radio actual de busqueda de fotones
		int nFotones; // num de fotones acumulados actual
		Color color; // Color actual hacia la camara

		Hit(const Vector3& _pos, const Vector3& _normal,
			const Vector3& _dir, const Color& _kd, int _iPixel, // Indice del pixel (orden i-d, arriba abajo)
			const Color& _peso, float _r, int _nFotones,
			const Color& _c) :
			pto(_pos), normal(_normal), dir(_dir),
			kd(_kd), iPixel(_iPixel),
			peso(_peso), r(_r), nFotones(_nFotones), color(_c)
		{}
	};

	int nFotonesTotal;

	std::vector<Hit> ptosVisibles; // Puntos visibles desde la camara

	// aux de preprocess, traza un rayo y almacena los hits en el vector de hits
	void traceRay(const Vector3& origen, const Vector3& dir,
	  int iPixel, float radio);

	// Lanza los rayos de la camara y los almacena en el vector de hits
	void preprocess();

	// Lanza fotones desde las luces y almacena fotones en los kdtrees
	void tracePhotons();
	// Añade la contribucion de los fotones guardados al vector de hits
	void addPhotonContribution(const float& alfa);
public:
	ProgressivePMRenderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo,
		const bool _usarBVH, const float _rangoDinamico=18, const int _maxNumFotones= 100000,
	  const int _maxFotonesGlobales= 100000, const int _maxFotonesCausticos= 0,
		const int _nFotonesCercanos = 50, const bool _guardarDirectos = true);

	// Se itera nVeces el proceso de tracePhotons y addPhotonContribution
	void iterar(int nVeces);

	// Se itera nVeces el proceso de tracePhotons y addPhotonContribution
	// Se guarda la imagen resultante de cada (iesima) iteracion en fichero-i.ppm
	void iterarRenderAll(int nVeces, const std::string& fichero);

	// Renderiza el vector de ptos visibles actual en el archivo <fichero>.ppm
	void render(const std::string& fichero);
	//TODO: implementar

};
