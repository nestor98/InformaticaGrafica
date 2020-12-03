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


PMRenderer::PMRenderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo, const bool _usarBVH, const float _rangoDinamico)
: Renderer(_e, _nThreads, tipo, _usarBVH, _rangoDinamico)
{
	// threads.reserve(_nThreads+1); // +1 por la barra de progreso
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

	//Iterate the path
	while(1)
	{
		// Throw ray and update current_it
		// Intersection it;
		auto inter = e.interseccion(oFoton, dirFoton);
		// first_intersection(photon_ray, it);

		if(!inter)
			break;
		Figura::InterseccionData iData = inter->first;
		Material mat = inter->second->getMaterial();
		//Check if has hit a delta material...
		if( mat.esDelta() )
		{
			// If delta material, then is caustic...
			// Don't store the photon!
			esCaustica = true;
		}
		else if (nivel > 0 || directo)
		{
			//If non-delta material, store the photon!
			if( esCaustica )
			{
				//If caustic particle, store in caustics
				if( fotonesCausticos.size() < maxFotonesCausticos )
					fotonesCausticos.emplace_back( Foton(iData.punto, dirFoton, energia ));
			}
			else
			{ // TODO: DE AQUI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//If non-caustic particle, store in global
				if( fotonesGlobales.size() < maxFotonesGlobales )
					fotonesGlobales.emplace_back( Foton(iData.punto, dirFoton, energia ));
			}
			esCaustica = false;
		}

		double pdf = 1; // TODO: ??????????????????????????????????????????????????

		// Color surf_albedo = mat.getCoeficiente(0); // Nos interesa el difuso //it.intersected()->material()->get_albedo(it);
		int evento = mat.ruletaRusa(rng, nivel==0);
		// Real avg_surf_albedo = surf_albedo.avg();
		//
		// Real epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
		// // while (epsilon2 < 0.)
		// // 	epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);

		if (evento == 0 || nivel > 1 ) // Absorcion
			break;

		// Random walk's next step
		// Get sampled direction plus pdf, and update attenuation
		auto fig = inter->second;
		Matriz4 base = fig->getBase(iData.punto);

		dirFoton = mat.getVectorSalida(base, rng, 0, false, dirFoton);
		oFoton = alejarDeNormal(iData.punto, base[2]); // Nuevo pto
		nivel++; // Un rebote mas

		// Shade...
		double albedo = mat.getCoeficiente(0).getPromedio();
		energia = energia*albedo;
		if( !mat.esDelta() )
			energia = energia * std::abs(base[2] * dirFoton)/PI;// base[2] es la normal

		energia = energia /(pdf*albedo);// pdf? :(
	}

	if( fotonesCausticos.size() == maxFotonesCausticos &&
		fotonesGlobales.size() == maxFotonesGlobales )
	{
		maxNumFotones = fotonesActuales-1; // ??????????????????????????????????
		return false;
	}

	return true;
}


template <class T, unsigned int N>
void guardarFotones(KDTree<T, N>& KDTFotones, const std::list<Foton>& fotones) {
	for (auto foton : fotones) {
		Vector3 pos = foton.getPos();
		std::vector<float> pto = {pos[0], pos[1], pos[2]};
		KDTFotones.store(pto, foton);
	}
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
void PMRenderer::preprocess()
{ // TODO: ejecutarlo D:
	std::vector<LuzPuntual> vLuces;
	e.getLuces(vLuces);
	GeneradorAleatorio rng;
	std::list<Foton> fotonesGlobales, fotonesCausticos;
	KDTree<Foton, MAX_FOTONES> KDTFotones;
	bool fin = false;
	while (!fin) {
    // std::cout << "iteracion: " << vLuces.size() << '\n';
		// sample
		int iLuz = rng.rand(0, vLuces.size()); // luz aleatoria
		LuzPuntual luz = e.getLuz(iLuz);
		Vector3 origen = luz.samplePunto(rng);
		Vector3 dir = rng.vectorNormalAleatorio();
		// trace
    // std::cout << "a trace..." << '\n';
		fin = !trace_ray(origen, dir, luz.getEmision(), fotonesGlobales,
							fotonesCausticos,	true, rng);
	}
	// store
	guardarFotones<Foton, MAX_FOTONES>(KDTFotones, fotonesGlobales);
	guardarFotones<Foton, MAX_FOTONES>(KDTFotones, fotonesCausticos);
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
  const std::shared_ptr<Figura>& figIntersectada) const
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

	case Renderer::TipoRender::Delta:
		// ----------------------------------------------------------------
		// Display whether the material is specular (or refractive)
    L.setRGB((!figIntersectada->esEmisor() && mat.esDelta()) ? 1 : 0); // Si es delta blanco, si no, negro
		// L = Vector3(it.intersected()->material()->is_delta());
		break;

	case Renderer::TipoRender::IluminacionLuz0:
		// ----------------------------------------------------------------
		// Display incoming illumination from light(0)
    L = Renderer::shadowRay(interseccion.punto, 0);
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
		if (!(Renderer::shadowRay(interseccion.punto, 0) == 0))
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

// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void PMRenderer::renderPixel(Imagen& im, const Vector3& o, const int pixel) const {
	Color color(0.0,0.0,0.0);
	auto c = e.getCamara();
	//int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
	GeneradorAleatorio rngThread; // generador para el thread
	//for (int i=0; i<nRayos; i++) { // cada rayo
	Vector3 dir(c->getRayoCentroPixel(pixel)); // una direccion
  auto intersec = e.interseccion(o, dir);
  if (intersec) {
    color = shade(intersec->first, intersec->second); // true para que el primero siempre rebote
  }
	im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}

void PMRenderer::render(const std::string fichero) {
	hrc::time_point t1, t2;
	t1 = hrc::now();
	// std::cout<<"a construir el arbol\n";
	if (usarBVH){
		std::vector<std::shared_ptr<Figura>> figs;
		e.getFiguras(figs);
		// std::cout << "A construir el bvh.." << '\n';
		bvh.construirArbol(figs);//figuras);
		std::cout<<"arbol bvh construido\n";
	}
	auto c = e.getCamara();
	Vector3 o = c->getPos();
	Imagen im(c->getPixelesY(), c->getPixelesX());
  // ---------------- PREPROCESS:
  std::cout << "Voy a hacer el preprocess" << '\n';
  //preprocess();
	t2 = hrc::now();
	std::chrono::duration<double> t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "\nPreprocess realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;
  // ---------------- RENDER:
  t1 = hrc::now();
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		//tasks.push_back(pixel); // encolar cada pixel
		PMRenderer::renderPixel(im, o, pixel);
	}
	// std::cout << "Inicializando threads... " << std::endl;
	// initThreads(im, o, c->getNumPixeles()); // inicializar los threads
	// // std::cout << "hecho" << '\n';
	// waitThreads(); // y esperar a que terminen
	im.setMaxFloat(rangoDinamico); // TODO: entender esta vaina
	im.extendedReinhard();
	im.guardar("out/" + fichero); // guardar la imagen

	t2 = hrc::now();
	t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "\nRender realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;
}
