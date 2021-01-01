#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include <chrono> // tests

#include "progressive-pm-renderer.hpp"


#define hrc std::chrono::high_resolution_clock


ProgressivePMRenderer::ProgressivePMRenderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo,
  const bool _usarBVH, const float _rangoDinamico, const int _maxNumFotones,
  const int _maxFotonesGlobales, const int _maxFotonesCausticos,
  const int _nFotonesCercanos, const bool _guardarDirectos)
: PMRenderer(_e, _nThreads, tipo, _usarBVH, _rangoDinamico,
  _maxNumFotones,_maxFotonesGlobales,_maxFotonesCausticos,
  _nFotonesCercanos,_guardarDirectos),
  nFotonesTotal(0)
{
	//threads.reserve(_nThreads+1); // +1 por la barra de progreso
  hrc::time_point t1, t2;
  std::chrono::duration<double> t; // duracion
	// std::cout<<"a construir el arbol\n";
	if (usarBVH) {
	  t1 = hrc::now();
    e.construirBVH();
    t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - hrc::now());
		std::cout<<"arbol bvh construido en " << t.count() <<" segundos\n";
	}
  // ---------------- PREPROCESS:
  std::cout << "Voy a hacer el preprocess" << '\n';
  t1 = hrc::now();
  preprocess();
	t2 = hrc::now();
	t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "\nPreprocess realizado en " << t.count()
            << " segundos (" << t.count()/60.0 << " minutos)"
            << std::endl;
}

void ProgressivePMRenderer::traceRay(const Vector3& origen, const Vector3& dir,
  int iPixel, float radio)
{
  auto intersec = e.interseccion(origen, dir);
  if (intersec) { // Ha intersectado
    auto iData = intersec->first;
    auto fig = intersec->second;
    bool evento = 0; // TODO: ruleta rusa, etc
    if (evento==0) {
      ptosVisibles.emplace_back(
        Hit(
          iData.punto, // posicion
          fig->getNormal(iData.punto), // normal
          dir, // direccion
          fig->getMaterial().getCoeficiente(evento), // kd
          iPixel, // Indice del pixel (orden i-d, arriba abajo)
          Color(), // Pixel weight (????????????)
          radio, // Radio actual de busqueda de fotones
          0, // num de fotones acumulados actual
          Color() // Color actual hacia la camara (0) TODO: asegurar

        )
      );
    }
  }
}

//*********************************************************************
// Lanza los rayos de la camara y los almacena en el vector de hits
void ProgressivePMRenderer::preprocess()
{
  auto camara = e.getCamara();
  auto o = camara->getPos();
  float radio = 1; // TODO: ??????????????????
  for (int iPixel = 0; iPixel < camara->getNumPixeles(); iPixel++) {
    for (int i = 0; i<camara->getRayosPorPixel(); i++) {
      traceRay(o, camara->getRayoPixel(iPixel), iPixel, radio);
    }
    //radio = radio*0.9; // TODO: revisar
  }
}

// Lanza fotones desde las luces y almacena fotones en los kdtrees
void ProgressivePMRenderer::tracePhotons() {
  kdTreeGlobal.clear();
  kdTreeCaustico.clear();
  // Trazar fotones y almacenar en los kdtrees:
  PMRenderer::preprocess();
  nFotonesTotal+=fotonesActuales;
}

// Añade la contribucion de los fotones guardados al vector de hits
void ProgressivePMRenderer::addPhotonContribution(const float& alfa) {
  for (auto& hit : ptosVisibles) {
    // TODO: tiene sentido usar dos kdtrees en progressive si se usan igual??
    int nFotCercanos = 0;
    hit.color = hit.color +
      hit.kd * PMRenderer::iluminacionRadioFijo(
        kdTreeGlobal, hit.pto, hit.normal, hit.r, nFotCercanos) +
      hit.kd * PMRenderer::iluminacionRadioFijo(
        kdTreeCaustico, hit.pto, hit.normal, hit.r, nFotCercanos);
    // Reduccion del radio (ecuacion 9, https://www.ci.i.u-tokyo.ac.jp/~hachisuka/ppm.pdf)
    // N = nFotones, M=nFotCercanos
    double dRsq = (hit.nFotones+alfa*nFotCercanos)/(hit.nFotones+nFotCercanos);
    hit.color = hit.color*dRsq; // Correccion del flujo (Ecuacion 12)
    hit.r *= sqrt(dRsq); // Ecuacion 9
    hit.nFotones += nFotonesCercanos;
  }
}


// Se itera nVeces el proceso de tracePhotons y addPhotonContribution
void ProgressivePMRenderer::iterar(int nVeces) {
  float alfa = 0.7; // TODO: miembro de la clase?
  for (int i = 0; i<nVeces; i++) {
    tracePhotons();
    addPhotonContribution(alfa);
  }
}

// Se itera nVeces el proceso de tracePhotons y addPhotonContribution
// Se guarda la imagen resultante de cada (iesima) iteracion en fichero-i.ppm
void ProgressivePMRenderer::iterarRenderAll(int nVeces, const std::string& fichero) {
  float alfa = 0.7; // TODO: miembro de la clase?
  for (int i = 0; i<nVeces; i++) {
    tracePhotons();
    addPhotonContribution(alfa);
    render(fichero+"-"+std::to_string(i+1));
  }

}

// Renderiza el vector de ptos visibles actual en el archivo <fichero>.ppm
void ProgressivePMRenderer::render(const std::string& fichero) {
  auto c = e.getCamara();
  Imagen imagen(c->getPixelesY(), c->getPixelesX());
  for (auto hit : ptosVisibles) {
    // TODO: habria que dividir el color entre el num de hits con iPixel?
    Color c = hit.color/(PI*hit.r*hit.r*nFotonesTotal);
    imagen.addToPixel(c[0],c[1],c[2],hit.iPixel);
  }
  imagen.extendedReinhard();
  imagen.guardar(fichero+".ppm");
}
