#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include <chrono> // tests

#include "pm-renderer.hpp"
#include "Foton.hpp"


#include "escena.hpp"



#define hrc std::chrono::high_resolution_clock


PMRenderer::PMRenderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo,
  const bool _usarBVH, const float _rangoDinamico, const int _maxNumFotones,
  const int _maxFotonesGlobales, const int _maxFotonesCausticos,
  const int _nFotonesCercanos, const bool _guardarDirectos)
: Renderer(_e, _nThreads, tipo, _usarBVH, _rangoDinamico),
  maxNumFotones(_maxNumFotones), maxFotonesGlobales(_maxFotonesGlobales),
  maxFotonesCausticos(_maxFotonesCausticos), fotonesActuales(0),
  nFotonesCercanos(_nFotonesCercanos), guardarDirectos(_guardarDirectos)
{
  std::cout << "Guardar dir: " << guardarDirectos << '\n';
  std::cout << "maxFotonesCausticos: " << maxFotonesCausticos << '\n';
  std::cout << "Nthreads: " << _nThreads << '\n';
	//threads.reserve(_nThreads+1); // +1 por la barra de progreso
}



/**************************************** PHOTON MAPPING *************************************/

//*********************************************************************
// Compute the photons by tracing the Ray 'r' from the light source
// through the scene, and by storing the intersections with matter
// in the lists 'xx_photons', storing the diffuse (global) and caustic
// photons respectively. For efficiency, both are computed at the same
// time, since computing them separately would result into a lost of
// several samples marked as caustic or diffuse.
// Same goes with the boolean 'directo', that specifies if directo
// photons (from light to surface) are being stored or not.
// The initial traced photon has energia defined by the tristimulus
// 'p', that accounts for the emitted power of the light source.
// The function will return true when there are more photons (caustic
// or diffuse) to be shot, and false otherwise.
//---------------------------------------------------------------------
// bool PMRenderer::trace_ray(const Ray& r, const Vector3 &p,
// 			   std::list<Foton> &fotonesGlobales, std::list<Foton> &fotonesCausticos, bool directo)

 bool PMRenderer::trace_ray(const Vector3& origen, const Vector3& dir, const Color& p,
 			   std::list<Foton> &fotonesGlobales, std::list<Foton> &fotonesCausticos, bool directo,
			 	const GeneradorAleatorio& rng)
{

	//Check if max number of shots done...
	if( ++fotonesActuales > maxNumFotones )
	{
		return false;
	}

	// Compute irradiance photon's energia
	// Vector3 energia(p);
	Color energia(p);

	// Ray photon_ray(r);
	// photon_ray.shift();
	Vector3 oFoton = origen;
	Vector3 dirFoton = dir;

	int nivel = 0; // num de rebotes // TODO: ir aumentandolo!!!!!!!!!!!!!!!!!1111

	bool esCaustica = false;
  bool todosCausticos = true;

	//Iterate the path
	while(1)
	{
		// Throw ray and update current_it
		// Intersection it;
		auto inter = e.interseccion(oFoton, dirFoton);
		// first_intersection(photon_ray, it);

		if(!inter)
			break;
    //std::cout << "he intersectado" << '\n';
		Figura::InterseccionData iData = inter->first;
		Material mat = inter->second->getMaterial();

    int evento = mat.ruletaRusa(rng, true); // No absorbemos con esta ruleta

    //Check if has hit a delta material...
		if( mat.esDelta(evento) )
		{
      esCaustica = todosCausticos;
			// If delta material, then is caustic...
			// Don't store the photon!

      //std::cout << "CAUSTICA" << '\n';
		}
		else {
      if (nivel > 0 || directo)
  		{
  			//If non-delta material, store the photon!
  			if( esCaustica )
  			{
  				//If caustic particle, store in caustics
  				if( fotonesCausticos.size() < maxFotonesCausticos ) {
            // std::cout << "meto un foton caustico" << '\n';
            fotonesCausticos.emplace_back( Foton(iData.punto, dirFoton, energia ));
          }
  			}
  			else
  			{
  				//If non-caustic particle, store in global
  				if( fotonesGlobales.size() < maxFotonesGlobales ) {
            // std::cout << "meto un foton global" << '\n';
    				fotonesGlobales.emplace_back( Foton(iData.punto, dirFoton, energia ));
          }
  			}
  		}
      todosCausticos = false;
      esCaustica = false;
    }

    Color albedo = mat.getCoeficiente(evento); // TODO: REVISAR 0
    double albedoPromedio = albedo.getPromedio();

		// Color surf_albedo = mat.getCoeficiente(0); // Nos interesa el difuso //it.intersected()->material()->get_albedo(it);
		//int evento = mat.ruletaRusa(rng, nivel==0);

		// Real avg_surf_albedo = surf_albedo.avg();
		//
		// Real epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
		// // while (epsilon2 < 0.)
		// // 	epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);

    double eps = rng.rand01();
		//if (evento == 3 || nivel > 20 || energia == 0 ) {// Absorcion 20
    if (eps > albedoPromedio || nivel > 20 ) {// Absorcion 20 || energia == 0

      // std::cout << "absorcion" << '\n';
      //if (energia == 0) std::cout << "energia 0" << '\n';
    	break;

    }
    // TODO: borrar esto
    // if (evento == 1 || evento == 2) {
    //   std::cerr << "DELTA sin implementar!!!!" << '\n';
    //   exit(1);
    // }
    double pdf = mat.getPDF(evento, true); // probabilidad del evento

    //int evento = 0;//mat.ruletaRusa(rng, );
		// Random walk's next step
		// Get sampled direction plus pdf, and update attenuation
		auto fig = inter->second;
		Matriz4 base = fig->getBase(iData.punto);

		dirFoton = mat.getVectorSalida(base, rng, evento, dirFoton);
    // TODO: al añadir refraccion cuidado con alejar el pto de la normal en este
    // sentido!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    if (evento == 2 && dirFoton*base[2] < 0) {
      // Si es transmision y se transmite se aleja pal otro lado
      oFoton = alejarDeNormal(iData.punto, -base[2]);
    }
    else {
  		oFoton = alejarDeNormal(iData.punto, base[2]); // Nuevo pto
    }
		nivel++; // Un rebote mas
    // if (nivel > 3) std::cout << "nivel>3" << '\n';

		// Shade...
    // double albedoPromedio = albedo.getPromedio();
    //double albedoMax = mat.getMax
    Color eAntes = energia;
		energia = energia*albedo;
		if( !mat.esDelta(evento) )
			energia = energia * std::abs(base[2] * dirFoton)/PI;// base[2] es la normal

		energia = energia /(pdf*albedoPromedio);//albedo.getMax());//*albedoPromedio);// pdf? :(
  }
	if( fotonesCausticos.size() == maxFotonesCausticos &&
		fotonesGlobales.size() == maxFotonesGlobales )
	{
		maxNumFotones = fotonesActuales-1; // ??????????????????????????????????
		return false;
	}
  // std::cout << nivel << " iteraciones en trace_ray, en preprocess" << '\n';
	return true;
}

template <class T, unsigned int N>
void guardarFotones(KDTree<T, N>& KDTFotones, const std::list<Foton>& fotones,
  const double& escalarPFotones) {
	for (auto foton : fotones) {
    //std::cout << "Guardando foton" << '\n';
		Vector3 pos = foton.getPos();
    foton.setEmision(foton.getEmision() * escalarPFotones);
    //std::cout << foton.getEmision(); // TODO: borrar
		std::vector<float> pto;
    pos.toKDTreePoint(pto); // = {pos[0], pos[1], pos[2]};
		KDTFotones.store(pto, foton);
	}
  if (!fotones.empty()) KDTFotones.balance();
}

//*********************************************************************
// TODO: Implement the preprocess step of photon mapping,
// where the photons are traced through the scene. To do it,
// you need to follow these steps for each shoot:
//  1 - Sample a world's light source in the scene to create
//		the initial directo photon from the light source.
//	2 - Trace the photon through the scene storing the inter-
//		sections between the photons and matter. You can use
//		the function 'trace_ray' for this purpose.
//	3 - Finally, once all the photons have been shot, you'll
//		need to build the photon maps, that will be used later
//		for rendering.
//		NOTE: Careful with function
//---------------------------------------------------------------------
void PMRenderer::preprocess(bool normalizar)
{
	std::vector<LuzPuntual> vLuces;
	e.getLuces(vLuces);
	GeneradorAleatorio rng;
	std::list<Foton> fotonesGlobales, fotonesCausticos;
	//KDTree<Foton, 3> KDTFotones;
	bool fin = false;
  int i = 0;
  // Esperamos iterar el sig. num de veces (10 es la p de absorcion):
  double esperanza_iteraciones = double(maxNumFotones)/10.0;
  std::cout << "esperamos iterar: " << esperanza_iteraciones << '\n';
  LuzPuntual luz0 = e.getLuz(0);
  std::cout << "Emision luz: " << luz0.getEmision() << " en cada iter: "
            << (luz0.getEmision()/esperanza_iteraciones) << '\n';
	while (!fin) {
    i++; // DEBUG
		// sample
		int iLuz = rng.rand(0, vLuces.size()); // luz aleatoria
		LuzPuntual luz = e.getLuz(iLuz);
		Vector3 origen = luz.samplePunto(rng,0);
    // std::cout << "muestra pto luz: "<<origen << '\n';
		Vector3 dir = rng.vectorNormalAleatorio();
    // std::cout << "dir: " <<dir << '\n';
		// trace
    // std::cout << "a trace..." << '\n';

		fin = !trace_ray(origen, dir, luz.getEmision(), fotonesGlobales,
							fotonesCausticos,	guardarDirectos, rng);//guardarDirectos
	}
  std::cout << i << " iteraciones de trace_ray (en preprocess)" << '\n';
	// store
  std::cout << "globales: " << fotonesGlobales.size() << "\ncausticos: "
            << fotonesCausticos.size() << '\n';
  double escalarPFotones = 1;
  if (normalizar) {
    escalarPFotones=4.0*PI/i; // cada foton debe tener 1/numfotones la e original
  }
	guardarFotones<Foton, 3>(kdTreeGlobal, fotonesGlobales, escalarPFotones);
	guardarFotones<Foton, 3>(kdTreeCaustico, fotonesCausticos, escalarPFotones);
}

// TODO: implementar...
Color PMRenderer::causticas(const Vector3& pto, const Vector3& normal) const
{
  int nCercanos; // No se usa
  return iluminacionRadioFijo(kdTreeCaustico, pto, normal, 0.05, nCercanos);
  // Color L;
  // float maxDist;
  // std::vector<float> ptoKDT;
  // pto.toKDTreePoint(ptoKDT);// pto para buscar en el KDTree
  // return iluminacionDeKDTree(1, pto, normal, 0.2);
}

Color PMRenderer::iluminacionRadioFijo(const KDTree<Foton,3>& kdTree,
  const Vector3& pto, const Vector3& normal,
  const float radio, int& nFotonesCercanos) const
{
  Color L;
  std::vector<float> ptoKDT;
  pto.toKDTreePoint(ptoKDT);// pto para buscar en el KDTree
  std::list<const KDTree<Foton, 3>::Node*> nodes;
  int cercanos = kdTreeCaustico.find(ptoKDT, radio, &nodes);
  for (auto node : nodes) {
    Foton foton = node->data();
    Vector3 dirFoton = foton.getDir(); // solo se tiene en cuenta si el foton
    // ha chocado con esta cara de la fig (normal*dir < 0)
    //if (dirFoton * figIntersectada->getNormal(interseccion.punto) < 0) {
    L = L + foton.getEmision() * std::abs(normal * dirFoton);// * std::abs(normal * dirFoton);// / (PI* (maxDist*maxDist));// * rCuadrado); // sum(flujo/(PI*r^2))
      //std::cout << foton.getEmision() << " ";
    //}
  }
  nFotonesCercanos+=cercanos;
  if (radio>0)
    L = L/(PI * (radio*radio));
  return  L;
}

Color PMRenderer::iluminacionGlobal(const Vector3& pto, const Vector3& normal) const
{
  Color L;
  float maxDist;
  std::vector<float> ptoKDT;
  pto.toKDTreePoint(ptoKDT);// pto para buscar en el KDTree
  std::vector<const KDTree<Foton, 3>::Node*> nodes;
  kdTreeGlobal.find(ptoKDT, nFotonesCercanos, nodes, maxDist);
  for (auto node : nodes) { // para cada foton
    Foton foton = node->data();
    Vector3 dirFoton = foton.getDir(); // solo se tiene en cuenta si el foton
    // TODO: probar estas lineas comentadas, igual quita bias:
    // ha chocado con esta cara de la fig (normal*dir < 0)
    //if (dirFoton * figIntersectada->getNormal(interseccion.punto) < 0) {
    L = L + foton.getEmision() * std::abs(normal * dirFoton);// / (PI* (maxDist*maxDist));// * rCuadrado); // sum(flujo/(PI*r^2))
      //std::cout << foton.getEmision() << " ";
    //}
  }
  if (nodes.size()>0)
    L = L / (PI * (maxDist*maxDist));
  return L;
}


Color PMRenderer::shadePM(const Figura::InterseccionData& interseccion,
  const std::shared_ptr<Figura>& figIntersectada, const bool primerRebote,
  const GeneradorAleatorio& rng, const Vector3& dir) const
{
  Color L;
  Material mat = figIntersectada->getMaterial();
  GeneradorAleatorio rngThread; //TODO: threads y tal
  // true pq es el primer rebote:
  int evento = mat.ruletaRusa(rng, primerRebote); // devuelve un entero entre 0 y 4 en f de las probs
  if (evento == 3) { // Absorcion, imposible en el 1er rebote
    return L;
  }
  else if (evento == 0) { // DIFUSO
    Vector3 n = figIntersectada->getNormal(interseccion.punto);
    Vector3 ptoCorregido = alejarDeNormal(interseccion.punto, n);
    L = iluminacionGlobal(interseccion.punto, n) +
        causticas(interseccion.punto, n);
    // L = L/2.0;
    //iluminacionGlobal(interseccion, n) + causticas(interseccion, n);
    if (!guardarDirectos) {
      //std::cout << "??????????" << '\n';
      L = L + Renderer::luzDirecta(ptoCorregido, n);
    }
    //L = L * mat.getCoeficiente(0);

    if (figIntersectada->tieneTextura()) { // con textura
			L= L*figIntersectada->getEmision(interseccion.punto); // El coeficiente es el de la textura
		}
		else { // difuso sin textura
			L = L * mat.getCoeficiente(0);
		}


  }
  else if (evento == 1) { // ESPECULAR
    Matriz4 base;Vector3 wi;
    try{
      base = figIntersectada->getBase(interseccion.punto);
      Vector3 ptoCorregido = alejarDeNormal(interseccion.punto, base[2], 1e-2);
  	  L = mat.getCoeficiente(0); // usamos el coeficiente del difuso
  		if (L == double(0)) L = mat.getCoeficiente(1);
  		wi = mat.getVectorSalida(base, rng, evento, dir);
      auto intersec = e.interseccion(ptoCorregido, wi);
      if (intersec) {
        L = L * shade(intersec->first, intersec->second, false, rng, wi); // true para que el primero siempre rebote
      }
    }
    catch (std::string e) {
      std::cerr << "Error en reflexion: " << e << '\n';
      std::cerr << "Base:\n" << base << "\nwi: " << wi << '\n';
      std::cerr << "dir: " << dir << '\n';
      std::cerr << "--------------------------" << '\n';

    }

  }
  else if (evento == 2) { // REFRACCION
    Matriz4 base;Vector3 wi;
    try {
  		// Matriz4 base = baseFromVectorYOrigen(fig->getNormal(pto), pto, dir);
  		base = figIntersectada->getBase(interseccion.punto);
      Vector3 ptoCorregido = alejarDeNormal(interseccion.punto, -base[2]);
  		L = mat.getCoeficiente(evento); // coef de refraccion en 0..0.9
  		L = L/0.9; // pasa a ser de 0 a 1. TODO: preguntar si se puede hacer esto
  		// std::cout << "c: " << c.to_string() << '\n';
      try {
    		wi = mat.getVectorSalida(base, rng, evento, dir);
        if (wi*base[2] > 0) {
          ptoCorregido = alejarDeNormal(interseccion.punto, base[2]);
        }
      }
      catch (std::string e) {
        std::cerr << "Error en getVectorSalida" << '\n';
      }
      auto intersec = e.interseccion(ptoCorregido, wi);
      if (intersec) {
        L = L * shade(intersec->first, intersec->second, false, rng, wi); // true para que el primero siempre rebote
      }
    }
    catch (std::string e) {
      std::cerr << "Error en refraccion: " << e << '\n';
      std::cerr << "Base:\n" << base << "wi: " << wi << '\n';
      std::cerr << "dir: " << dir << '\n';
      std::cerr << "--------------------------" << '\n';
    }
  }
  return L;
}

// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void PMRenderer::renderPixel(Imagen& im, const Vector3& o, const int pixel,
const GeneradorAleatorio& rng) const {
	Color color(0.0,0.0,0.0);
	auto c = e.getCamara();
	int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
	for (int i=0; i<nRayos; i++) { // cada rayo
    Vector3 dir;
    if (nRayos == 1) { // un rayo, por el centro
      dir = c->getRayoCentroPixel(pixel);
    }
    else {
  	   dir = c->getRayoPixel(pixel); // dir aleatoria
    }

    auto intersec = e.interseccion(o, dir);
    if (intersec) {
      color = color + shade(intersec->first, intersec->second,
      true, rng, dir); // true para que el primero siempre rebote
    }
  }
  color = color / nRayos;
	im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}



//*********************************************************************
// TODO: Implement the function that computes the rendering equation
// using radiance estimation with photon mapping, using the photon
// maps computed as a proprocess. Note that you will need to handle
// both direct and global illumination, together with the
// recursive evaluation of delta materials. For an optimal implemen-
// tation you should be able to do it iteratively.
// In principle, the class is prepared to perform radiance estimation
// using k-nearest neighbors ('m_nb_photons') to define the bandwidth
// of the kernel.
//---------------------------------------------------------------------
Color PMRenderer::shade(const Figura::InterseccionData& interseccion,
  const std::shared_ptr<Figura>& figIntersectada, const bool primerRebote,
  const GeneradorAleatorio& rng, const Vector3& dir) const
{
	Color L(0);
	//Intersection it(it0);

	//**********************************************************************
	// The following piece of code is included here for two reasons: first
	// it works as a 'hello world' code to check that everthing compiles
	// just fine, and second, to illustrate some of the functions that you
	// will need when doing the work. Goes without saying: remove the
	// pieces of code that you won't be using.
	//
	// unsigned int debug_mode = 1;
  Material mat = figIntersectada->getMaterial();
	switch ((int) renderSeleccionado)
	{
  case Renderer::TipoRender::Materiales:
    L = shadePM(interseccion, figIntersectada,primerRebote,rng,dir);
    break;
  case Renderer::TipoRender::FotonesRadioFijo:
  {
    std::vector<float> pto;
    interseccion.punto.toKDTreePoint(pto);// pto para buscar en el KDTree
    std::list<const KDTree<Foton, 3>::Node*> nodes;
    float maxDist;
    //std::cout << "nFotonesCercanos: "<< nFotonesCercanos << '\n';
    float radioAOjo = 2  * 1e-2;
    int cercanos = kdTreeGlobal.find(pto, radioAOjo, &nodes);
    // kdTreeGlobal.find(pto, nFotonesCercanos, nodes, maxDist);
    // std::cout << "cercanos: " <<cercanos << '\n';
    L.setRGB(cercanos);
    break;
  }
  case Renderer::TipoRender::FotonMasCercano:
  {
    std::vector<float> pto;
    interseccion.punto.toKDTreePoint(pto);// pto para buscar en el KDTree
    //std::cout << "pto: " << pto[0] << " " << pto[1] << " " << pto[2] << '\n';
    std::vector<const KDTree<Foton, 3>::Node*> nodes;
    float maxDist;
    //std::cout << "nFotonesCercanos: "<< nFotonesCercanos << '\n';
    kdTreeGlobal.find(pto, 1, nodes, maxDist);
    //std::cout << nodes[0]->data().getEmision().to_string() << std::endl;
    L = nodes[0]->data().getEmision()/maxDist;///float(maxDist*maxDist); // Color del mas cercano / distancia
    break;
  }
  case Renderer::TipoRender::FotonCausticoMasCercano:
  {
    std::vector<float> pto;
    interseccion.punto.toKDTreePoint(pto);// pto para buscar en el KDTree
    //std::cout << "pto: " << pto[0] << " " << pto[1] << " " << pto[2] << '\n';
    std::vector<const KDTree<Foton, 3>::Node*> nodes;
    float maxDist;
    //std::cout << "nFotonesCercanos: "<< nFotonesCercanos << '\n';
    kdTreeCaustico.find(pto, 1, nodes, maxDist);
    //std::cout << nodes[0]->data().getEmision().to_string() << std::endl;
    L = nodes[0]->data().getEmision()/maxDist;///float(maxDist*maxDist); // Color del mas cercano / distancia
    break;
  }
	case Renderer::TipoRender::Albedo:
		// ----------------------------------------------------------------
		// Display Albedo Only
		L = mat.getCoeficiente(0);
    // it.intersected()->material()->get_albedo(it);
		break;
	case Renderer::TipoRender::Normales:
    {
      // ----------------------------------------------------------------
  		// Display Normal Buffer
      Vector3 normal = figIntersectada->getNormal(interseccion.punto);
      L.setFromNormalVector(normal);
      // L = interseccion.
  		// L = it.get_normal();
  		break;
    }
  case Renderer::TipoRender::VectoresAleatorios:
    {
      // ----------------------------------------------------------------
  		// Muestra la dir de los vectores de la luz 0
      GeneradorAleatorio rng;
      L.setFromNormalVector(rng.vectorNormalAleatorio());
      // L = interseccion.
  		// L = it.get_normal();
  		break;
    }

	case Renderer::TipoRender::Delta:
		// ----------------------------------------------------------------
    // TODO: si eso, tampoco hace falta
		// Display whether the material is specular (or refractive)
    // evento = mat.ruletaRusa(rng,)
    // L.setRGB((!figIntersectada->esEmisor() && mat.esDelta()) ? 1 : 0); // Si es delta blanco, si no, negro
		// L = Vector3(it.intersected()->material()->is_delta());
		break;

	case Renderer::TipoRender::IluminacionLuz0:
		// ----------------------------------------------------------------
		// Display incoming illumination from light(0)
    L = Renderer::shadowRay(interseccion.punto, figIntersectada->getNormal(interseccion.punto), 0);
		// L = world->light(0).get_incoming_light(it.get_position());
		break;

	case Renderer::TipoRender::DireccionLuz0:
		// ----------------------------------------------------------------
		// Display incoming direction from light(0)
    L.setFromNormalVector(normalizar(e.getLuz(0).getPos() - interseccion.punto));
		// L = world->light(0).get_incoming_direction(it.get_position());
		break;

	case Renderer::TipoRender::VisibilidadLuz0:
		// ----------------------------------------------------------------
		// Check Visibility from light(0)
		if (!(Renderer::shadowRay(interseccion.punto, figIntersectada->getNormal(interseccion.punto), 0) == 0))
			L.setRGB(1);
		break;
  default:
    std::cerr << "Tipo render sin implementar en PM" << '\n';
    exit(1);
	}
	// End of exampled code
	//**********************************************************************

	return L;
}



void PMRenderer::render(const std::string fichero) {
	hrc::time_point t1, t2;
  std::chrono::duration<double> t; // duracion
	// std::cout<<"a construir el arbol\n";
	if (usarBVH){
	  t1 = hrc::now();
    e.construirBVH();
    t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - hrc::now());
		std::cout<<"arbol bvh construido en " << t.count() <<" segundos\n";
	}
	auto c = e.getCamara();
	Vector3 o = c->getPos();
	Imagen im(c->getPixelesY(), c->getPixelesX());
  // ---------------- PREPROCESS:
  std::cout << "Voy a hacer el preprocess" << '\n';
  t1 = hrc::now();
  preprocess();
	t2 = hrc::now();
	t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "\nPreprocess realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;
  // ---------------- RENDER:
  t1 = hrc::now();
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		tasks.push_back(pixel); // encolar cada pixel
		//PMRenderer::renderPixel(im, o, pixel);
	}
	std::cout << "Inicializando threads... " << std::endl;
	initThreads(im, o, c->getNumPixeles()); // inicializar los threads
	// // std::cout << "hecho" << '\n';
	Renderer::waitThreads(); // y esperar a que terminen
	im.setMaxFloat(rangoDinamico); // TODO: entender esta vaina
	im.extendedReinhard();
	im.guardar("out/" + fichero); // guardar la imagen

	t2 = hrc::now();
	t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "\nRender realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;
}

/********************************** PARALELIZACION **********************************/

void PMRenderer::consumirTasks(Imagen& im, const Vector3& origen) {
	//std::cout<<"Bueno"<<std::endl;
	// int cuenta = 0;
  GeneradorAleatorio rngThread;
	while (true) {
		int pixel;
		{ //Lock
			// Las llaves son para que la guarda solo este entre ellas (scope):
			std::lock_guard<std::mutex> guarda(mtx); // asegura la SC
			if (tasks.empty()) { // Fin cuando no quedan tasks
				break;
			}
			pixel = tasks.back();
			tasks.pop_back();
		} // unlock
		PMRenderer::renderPixel(im, origen, pixel,rngThread);
		// cuenta++;
	}
	// std::cout<<"He dibujado: " << cuenta << " pixeles\n";
}

void PMRenderer::initThreads(Imagen& im, const Vector3& origen, const int nPixeles) {
	for (int i=0; i<threads.capacity()-1; i++) {
		// std::thread t1(&Escena::consumirTasks, this, std::ref(im), std::ref(origen));
		threads.emplace_back(std::thread(&PMRenderer::consumirTasks, this, std::ref(im), std::ref(origen)));
	}
	threads.emplace_back(std::thread(&Renderer::progressBar, this, nPixeles));
}
//
// void PMRenderer::waitThreads() {
// 	for (int i=0; i<threads.capacity(); i++) {
// 		threads[i].join();
// 	}
// }




/* Version mas como la proporcionada:

		// Color surf_albedo = mat.getCoeficiente(0); // Nos interesa el difuso //it.intersected()->material()->get_albedo(it);
		//int evento = mat.ruletaRusa(rng, nivel==0);
    double pdf = mat.getPDF(0, nivel==0); // probabilidad del evento
		// Real avg_surf_albedo = surf_albedo.avg();
		//
		// Real epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
		// // while (epsilon2 < 0.)
		// // 	epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);

    Color albedo = mat.getCoeficiente(0); // TODO: REVISAR 0
    double albedoPromedio = albedo.getPromedio();
    double eps = rng.rand01();
    int evento = 0;
		//if (evento == 3 || nivel > 20 || energia == 0 ) {// Absorcion 20
    if (eps > albedoPromedio || nivel > 20 || energia == 0 ) {// Absorcion 20

      // std::cout << "absorcion" << '\n';
      if (energia == 0) std::cout << "energia 0" << '\n';
    	break;

    }
    // TODO: borrar esto
    // if (evento == 1 || evento == 2) {
    //   std::cerr << "DELTA sin implementar!!!!" << '\n';
    //   exit(1);
    // }


		// Random walk's next step
		// Get sampled direction plus pdf, and update attenuation
		auto fig = inter->second;
		Matriz4 base = fig->getBase(iData.punto);

		dirFoton = mat.getVectorSalida(base, rng, evento, dirFoton);
    // TODO: al añadir refraccion cuidado con alejar el pto de la normal en este
    // sentido!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		oFoton = alejarDeNormal(iData.punto, base[2]); // Nuevo pto
		nivel++; // Un rebote mas
    // if (nivel > 3) std::cout << "nivel>3" << '\n';

		// Shade...
    // double albedoPromedio = albedo.getPromedio();
    //double albedoMax = mat.getMax
    Color eAntes = energia;
		energia = energia*albedo;
		if( !mat.esDelta() )
			energia = energia * std::abs(base[2] * dirFoton)/PI;// base[2] es la normal

		energia = energia /(pdf*albedoPromedio);//albedo.getMax());//*albedoPromedio);// pdf? :(
*/






/* Con maximos en vez de promedios:

    // ------------------------------------------------
		// Color surf_albedo = mat.getCoeficiente(0); // Nos interesa el difuso //it.intersected()->material()->get_albedo(it);
		int evento = mat.ruletaRusa(rng);
    double pdf = mat.getPDF(evento); // probabilidad del evento
		// Real avg_surf_albedo = surf_albedo.avg();
		//
		// Real epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
		// // while (epsilon2 < 0.)
		// // 	epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);

    //Color albedo = mat.getCoeficiente(0); // TODO: REVISAR 0
    //double albedoPromedio = albedo.getPromedio();
    //double eps = rng.rand01();
    //int evento = 0;
		if (evento == 3 || nivel > 20 || energia == 0 ) {// Absorcion 20
    //if (eps > albedoPromedio || nivel > 20 || energia == 0 ) {// Absorcion 20

      // std::cout << "absorcion" << '\n';
      if (energia == 0) std::cout << "energia 0" << '\n'; // TODO: borrar
    	break;

    }
    // TODO: borrar esto
    if (evento == 1 || evento == 2) {
      std::cerr << "DELTA sin implementar!!!!" << '\n';
      exit(1);
    }
		// Random walk's next step
		// Get sampled direction plus pdf, and update attenuation
		auto fig = inter->second;
		Matriz4 base = fig->getBase(iData.punto);

		dirFoton = mat.getVectorSalida(base, rng, evento, dirFoton);
    // TODO: al añadir refraccion cuidado con alejar el pto de la normal en este
    // sentido!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		oFoton = alejarDeNormal(iData.punto, base[2]); // Nuevo pto
		nivel++; // Un rebote mas
    // if (nivel > 3) std::cout << "nivel>3" << '\n';
    // Nueva energia = energia * kd / pdf
    energia = energia * mat.getCoeficiente(evento) / pdf;
    if( !mat.esDelta() )
    	energia = energia * std::abs(base[2] * dirFoton)/PI;// base[2] es la normal

	}
  */
