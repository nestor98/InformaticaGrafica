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

 bool PMRenderer::trace_ray(const Escena& e, const Vector3& origen, const Vector3& dir, const Color& p,
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

		if (evento == 0 || nivel > 20 ) // Absorcion
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
void PMRenderer::preprocess(const Escena& e)
{ // TODO: ejecutarlo D:
	std::vector<LuzPuntual> vLuces;
	e.getLuces(vLuces);
	GeneradorAleatorio rng;
	std::list<Foton> fotonesGlobales, fotonesCausticos;
	KDTree<Foton, MAX_FOTONES> KDTFotones;
	bool fin = false;
	while (!fin) {
		// sample
		int iLuz = rng.rand(0, vLuces.size()); // luz aleatoria
		LuzPuntual luz = e.getLuz(iLuz);
		Vector3 origen = luz.samplePunto(rng);
		Vector3 dir = rng.vectorNormalAleatorio();
		// trace
		fin = !trace_ray(e, origen, dir, luz.getEmision(), fotonesGlobales,
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


// TODO: constructor..........................

// Renderer::Renderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo, const bool _usarBVH)
// : usarBVH(_usarBVH), e(_e), renderSeleccionado(tipo)//, threads(_nThreads)
// {
// 	threads.reserve(_nThreads+1); // +1 por la barra de progreso
// }


//
// Renderer::Renderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo, const bool _usarBVH)
// : usarBVH(_usarBVH), e(_e), renderSeleccionado(tipo)//, threads(_nThreads)
// {
// 	threads.reserve(_nThreads+1); // +1 por la barra de progreso
// }
//
// Color Renderer::Renderer::shadowRay(const Vector3& pto, const int indiceluz) const {
// 	Color c;
// 	LuzPuntual luz=	e.getLuz(indiceluz);
// 	Vector3 rayoSombra = luz.getPos() - pto;
// 	// std::cout << "ueyueya" << '\n';
// 	double distLuz = rayoSombra.getModulo();
// 	rayoSombra = normalizar(rayoSombra);
// 	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccionFigura;
// 	// std::cout << "ueyueya" << '\n';
// 	if (!usarBVH) { // Sin bvh
// 		interseccionFigura = e.interseccion(pto, rayoSombra);
// 	}
// 	else { // con bvh
// 		// std::cout << "Cuidado con la interseccion con bvh" << '\n';
// 		interseccionFigura = bvh.interseccion(pto, rayoSombra);
// 	}
// 	if (interseccionFigura) {
// 		if (distLuz < interseccionFigura->first.t) {
// 			return luz.getEmision() / (distLuz * distLuz); // Se devuelve su color entre la distancia al cuadrado
// 		}
// 	}
// 	return c;
// }
//
//
// Color Renderer::ruletaRusa(const std::shared_ptr<Figura> fig, const Vector3& dir, const Vector3& pto, const GeneradorAleatorio& rngThread, const bool primerRebote) const {
// 	Material mat = fig->getMaterial();
// 	Color c;
//
// 	int evento = mat.ruletaRusa(rngThread, primerRebote); // devuelve un entero entre 0 y 4 en f de las probs
// 	if (evento == 3) {// absorcion
// 		// std::cout << "absorcion.." << '\n';
// 		return c;
// 	}
// 	else if (evento == 2) { // REFRACCION
// 		// Matriz4 base = baseFromVectorYOrigen(fig->getNormal(pto), pto, dir);
// 		Matriz4 base = fig->getBase(pto);
//
// 		c = mat.getCoeficiente(evento); // coef de refraccion en 0..0.9
// 		c = c/0.9; // pasa a ser de 0 a 1. TODO: preguntar si se puede hacer esto
// 		// std::cout << "c: " << c.to_string() << '\n';
// 		// inside = dir * normal;
// 		bool inside = false;
// 		// if (dir * base[2] > 0) {
// 		// 	base[2] = -base[2];
// 		// 	inside = true;
// 		// }
// 		float kr;
// 		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, inside, dir, kr);
// 		c = pathTrace(pto+0.01*otroPath, otroPath, rngThread);// * kr; //
// 	}
// 	else if (evento == 1) { // ------------------------ REFLEXION
// 		Matriz4 base = fig->getBase(pto);
// 		c = mat.getCoeficiente(0); // usamos el coeficiente del difuso
// 		if (c == double(0)) c = mat.getCoeficiente(1);
// 		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, false, dir);
// 		c = c*pathTrace(pto+0.01*otroPath, otroPath, rngThread); // kd * Li
// 	}
// 	else { // --------------------------- DIFUSO
// 		if (fig->tieneTextura()) { // con textura
// 			c = fig->getEmision(pto);
// 		}
// 		else { // difuso sin textura
// 			c = mat.getCoeficiente(evento);
// 		}
// 		// Solo queda elegir entre iluminacion directa e indirecta:
// 		float probRayoSombra = 0.5; // TODO: ???????????????
// 		int numLuces = e.getNumLuces();
// 		/*if (numLuces > 0 && rngThread.rand01() < probRayoSombra) { // Iluminacion directa
// 			// Opcion 1 ------  ruleta rusa, una al azar
// 			int indiceluz = rngThread.rand(0, numLuces);
// 			c = c * Renderer::shadowRay(pto, indiceLuz);
// 			// std::cout << "1" << '\n';
// 			// Opcion 2 ------ Promedio:
// 			// Color ilum;
// 			// for (int i = 0; i<numLuces; i++) {
// 			// 	ilum = ilum + Renderer::shadowRay(pto, i);
// 			// }
// 			// c = c*ilum/double(numLuces);
// 			// std::cout << "2" << '\n';
// 		}
// 		else { // iluminacion indirecta
// 			Matriz4 base = fig->getBase(pto);
// 			Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, false, dir);
// 			c = c*pathTrace(pto+0.01*otroPath, otroPath, rngThread); // kd * Li
// 		}*/
// 		// Otra opcion, ambas a la vez:
// 		Color iDirecta;
// 		if (numLuces>0){
// 			int indiceLuz = rngThread.rand(0, numLuces);
// 			iDirecta = Renderer::shadowRay(pto, indiceLuz);
// 		}
// 		Matriz4 base = fig->getBase(pto);
// 		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, false, dir);
// 		c = c*(iDirecta + pathTrace(pto+0.01*otroPath, otroPath, rngThread)); // kd * Li
//
// 	}
//
//
// 	return c;
// }
//
//
//
// // Aux de path trace, solo para tipos de renders raros basados en vectores
// Vector3 vectorTipoRender(const Renderer::TipoRender tipoRender, const std::shared_ptr<Figura> fig, const Vector3& dir, const Vector3& ptoInterseccion, const GeneradorAleatorio& gen) {
// 	Material mat = fig->getMaterial();
// 	if (tipoRender == Renderer::VectoresWiRefraccion){
// 		// Matriz4 base = baseFromVectorYOrigen(fig->getNormal(ptoInterseccion), ptoInterseccion, dir);
// 			Matriz4 base = fig->getBase(ptoInterseccion);
// 		bool inside = false;
// 		if (dir * base[2] > 0) {
// 			base[2] = -base[2];
// 			inside = true;
// 		}
// 		// Vector3 otroPath =
// 		// std::cout << base.inversa() * dir << std::endl;
// 		Material mat = fig->getMaterial();
// 		return mat.getVectorSalida(base, gen, 2, inside, dir);
// 	}
// 	else if (tipoRender == Renderer::VectoresWiReflexion) {
// 		Matriz4 base = fig->getBase(ptoInterseccion);
// 		return mat.getVectorSalida(base, gen, 1,false, dir);
// 	}
// 	else if (tipoRender == Renderer::VectoresWiDifusos) {
// 		Matriz4 base = fig->getBase(ptoInterseccion);
// 		return mat.getVectorSalida(base, gen, 0,false, dir);
// 	}
// 	else if (tipoRender == Renderer::Normales) {
// 		return fig->getNormal(ptoInterseccion);
// 	}
// 	else {
// 		std::cerr << "ERROR: Tipo de render desconocido" << '\n';
// 		exit(1);
// 	}
// }
//
//
// Color Renderer::pathTrace(const Vector3& o, const Vector3& dir, const GeneradorAleatorio& rngThread, const bool primerRebote) const {
// 	// std::cout << "Trazando un path" << '\n';
// 	Color c = COLOR_FONDO;
// 	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccionFigura;
// 	// std::cout << "Voy a intersectar" << '\n';
// 	if (!usarBVH) { // Sin bvh
// 		interseccionFigura = e.interseccion(o, dir);
// 	}
// 	else { // con bvh
// 		// std::cout << "Cuidado con la interseccion con bvh" << '\n';
// 		interseccionFigura = bvh.interseccion(o, dir);
// 	}
// 	if (interseccionFigura) { // intersecta con alguna
// 		if (!primerRebote) {
// 			// std::cout << "No soy el primer rebote Y he intersectado con algo" << '\n';
// 		}
// 		auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
// 		// std::cout << "antes de emisor y blabnanjasfobnabsodlnaikjnaspian: " << fig << '\n';
// 		if (fig->esEmisor()) { // fin de la recursión, es un emisor
// 			// std::cout << "a por emision" << '\n';
// 			// return fig->getEmision();
// 			// if (fig->)
// 			c = fig->getEmision(interseccionFigura->first.punto);
// 			// if (!primerRebote) return c*2.0; // TODO: multiplicacion bestia de la iluminacion, revisar
// 		}
// 		else {
// 			Figura::InterseccionData iData = interseccionFigura->first;
// 			// std::cout << "A ver el pto" << '\n';
// 			// t = iData.t;
// 			Vector3 ptoInterseccion = iData.punto;
// 			if (renderSeleccionado == Materiales) { // Path trace normal
// 				// std::cout << "He intersectado con un no emisor" << '\n';
// 				c = ruletaRusa(fig, dir, ptoInterseccion, rngThread, primerRebote);
// 			}
// 			else if (renderSeleccionado == krFresnel) {
// 				double kr = fig->getMaterial().krFresnel(dir, fig->getNormal(ptoInterseccion), 1.45);
// 				c = Color(kr); // ya va de 0 a 1
// 			}
// 			else { // otro tipo de render:
// 				Vector3 vector = vectorTipoRender(renderSeleccionado, fig,  dir, ptoInterseccion,rngThread);
// 				c.setFromNormalVector(vector); // color del vector, cada comp en un canal rgb
// 			}
// 		}
// 	}
// 	return c;///(t*t);
// }
//
//
// // Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
// void Renderer::renderPixel(Imagen& im, const Vector3& o, const int pixel) const {
// 		Color color(0.0,0.0,0.0);
// 		auto c = e.getCamara();
// 		int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
// 		GeneradorAleatorio rngThread; // generador para el thread
// 		for (int i=0; i<nRayos; i++) { // cada rayo
// 			Vector3 dir(c->getRayoPixel(pixel)); // una direccion
// 			Color cPixel = pathTrace(o, dir, rngThread, true); // true para que el primero siempre rebote
// 			color = color + cPixel;// suma de cada path / double(nRayos);
// 		}
// 		color = color / double(nRayos); // promedio
// 		color.clamp(1.0); // TODO: revisar
// 		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
// }
//
//
//
//
//
// void Renderer::consumirTasks(Imagen& im, const Vector3& origen) {
// 	//std::cout<<"Bueno"<<std::endl;
// 	// int cuenta = 0;
// 	while (true) {
// 		int pixel;
// 		{ //Lock
// 			// Las llaves son para que la guarda solo este entre ellas (scope):
// 			std::lock_guard<std::mutex> guarda(mtx); // asegura la SC
// 			if (tasks.empty()) { // Fin cuando no quedan tasks
// 				break;
// 			}
// 			pixel = tasks.back();
// 			tasks.pop_back();
// 		} // unlock
// 		renderPixel(im, origen, pixel);
// 		// cuenta++;
// 	}
// 	// std::cout<<"He dibujado: " << cuenta << " pixeles\n";
// }
//
//
// // Barra de progreso adaptada de: https://stackoverflow.com/a/14539953
// void dibujarBarraProgreso(double porcentaje, int longitudBarra, hrc::time_point t_ini, hrc::time_point t_ahora) {
// 	int nCaracteres = porcentaje*longitudBarra; // numero de veces que se repite el caracter de la barra
// 	std::string caracter = "█"; // caracter de completado
//
// 	// std::cout  << "[" << std::flush;
// 	for (int i=0; i<longitudBarra; i++) {
// 		if (i<nCaracteres) { // Completados
// 			std::cout << caracter;
// 		}
// 		else {
// 			std::cout << "░";
// 		}
// 	}
// 	double t_elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t_ahora - t_ini).count();
// 	double s_restantes = 10;
// 	if (porcentaje>0) {
// 		s_restantes = (1.0-porcentaje)/porcentaje * t_elapsed;
// 	}
// 	std::cout << " " << int(porcentaje*100) << '%'<< " (quedan unos " << int(s_restantes)/60 << ":"<< int(s_restantes)%60 << " min)          \r" << std::flush;
// }
//
//
// void Renderer::progressBar(const int nPixeles) {
// 	hrc::time_point t1, t2;
// 	t1 = hrc::now();
// 	int longitudBarra = 50;
// 	while (true) {
// 		int nRestantes;
// 		{ //Lock
// 			// Las llaves son para que la guarda solo este entre ellas (scope):
// 			std::lock_guard<std::mutex> guarda(mtx); // asegura la SC
// 			if (tasks.empty()) { // Fin cuando no quedan tasks
// 				break;
// 			}
// 			nRestantes = tasks.size();
// 		} // unlock
// 		int nTerminados = nPixeles - nRestantes;
// 		double porcentaje = double(nTerminados)/double(nPixeles);
// 		t2 = hrc::now();
// 		dibujarBarraProgreso(porcentaje, longitudBarra, t1, t2);
// 		// dibujarBarraProgreso(porcentaje, longitudBarra);
// 		std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Duerme 2 s
// 	}
// 	dibujarBarraProgreso(1, longitudBarra, t1, t2);
// 	std::cout << std::endl;
// }
//
// void Renderer::initThreads(Imagen& im, const Vector3& origen, const int nPixeles) {
// 	for (int i=0; i<threads.capacity()-1; i++) {
// 		// std::thread t1(&Escena::consumirTasks, this, std::ref(im), std::ref(origen));
// 		threads.emplace_back(std::thread(&Renderer::consumirTasks, this, std::ref(im), std::ref(origen)));
// 	}
// 	threads.emplace_back(std::thread(&Renderer::progressBar, this, nPixeles));
// }
//
// void Renderer::waitThreads() {
// 	for (int i=0; i<threads.capacity(); i++) {
// 		threads[i].join();
// 	}
// }
//
// std::string Renderer::to_string() const {
// 	std::string s = "No has implementado el to_string de Renderer\n";
// 	return s;
// }
//
//
// void Renderer::render(const std::string fichero) {
// 	hrc::time_point t1, t2;
// 	t1 = hrc::now();
// 	// std::cout<<"a construir el arbol\n";
// 	if (usarBVH){
// 		std::vector<std::shared_ptr<Figura>> figs;
// 		e.getFiguras(figs);
// 		// std::cout << "A construir el bvh.." << '\n';
// 		bvh.construirArbol(figs);//figuras);
// 		std::cout<<"arbol bvh construido\n";
// 	}
// 	auto c = e.getCamara();
// 	Vector3 o = c->getPos();
// 	Imagen im(c->getPixelesY(), c->getPixelesX());
// 	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
// 		tasks.push_back(pixel); // encolar cada pixel
// 		//renderPixel(im, o, pixel);
// 	}
// 	std::cout << "Inicializando threads... " << std::endl;
// 	initThreads(im, o, c->getNumPixeles()); // inicializar los threads
// 	// std::cout << "hecho" << '\n';
// 	waitThreads(); // y esperar a que terminen
// 	// im.setMaxFloat(1); // TODO: entender esta vaina
// 	// im.extendedReinhard();
// 	im.guardar("out/" + fichero); // guardar la imagen
//
// 	t2 = hrc::now();
// 	std::chrono::duration<double> t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
// 	// t2 -> tRender2 = t2-t1
// 	std::cout << "\nRender realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;
// }
//
//
//
// // para evitar el to_string en cout
// std::ostream& operator<<(std::ostream& os, const Renderer& e) {
// 	os << e.to_string();
// 	return os;
// }
