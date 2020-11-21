#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include <chrono> // tests

#include "renderer.hpp"
#include "material.hpp"

#define hrc std::chrono::high_resolution_clock

const Color COLOR_FONDO(0,0,0); // TODO: mirar como ponerlo mas bonito



Renderer::Renderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo, const bool _usarBVH)
: usarBVH(_usarBVH), e(_e), renderSeleccionado(tipo)//, threads(_nThreads)
{
	threads.reserve(_nThreads+1); // +1 por la barra de progreso
}


Color Renderer::ruletaRusa(const std::shared_ptr<Figura> fig, const Vector3& dir, const Vector3& pto, const GeneradorAleatorio& rngThread,bool inside, const bool primerRebote) const {
	Material mat = fig->getMaterial();
	int evento = mat.ruletaRusa(rngThread, primerRebote); // devuelve un entero entre 0 y 4 en f de las probs
	Color c;
	if (evento == 3) {// absorcion
		// std::cout << "absorcion.." << '\n';
		return c;
	}
	else if (evento == 2) { // REFRACCION
		// Matriz4 base = baseFromVectorYOrigen(fig->getNormal(pto), pto, dir);
		Matriz4 base = fig->getBase(pto);

		c = mat.getCoeficiente(evento); // coef de refraccion en 0..0.9
		c = c/0.9; // pasa a ser de 0 a 1. TODO: preguntar si se puede hacer esto
		// std::cout << "c: " << c.to_string() << '\n';
		// inside = dir * normal;
		inside = false;
		if (dir * base[2] > 0) {
			base[2] = -base[2];
			inside = true;
		}
		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, inside, dir);
		c = pathTrace(pto+0.01*dir, otroPath, rngThread, !inside);
	}
	else { // REFLEXION o DIFUSO:
		Matriz4 base = fig->getBase(pto);
		c = mat.getCoeficiente(evento); // kd
		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, inside, dir);
		c = c*pathTrace(pto, otroPath, rngThread, inside); // kd * Li
	}
	return c;
}



// Aux de path trace, solo para tipos de renders raros basados en vectores
Vector3 vectorTipoRender(const Renderer::TipoRender tipoRender, const std::shared_ptr<Figura> fig, const Vector3& dir, const Vector3& ptoInterseccion, const GeneradorAleatorio& gen) {
	Material mat = fig->getMaterial();
	if (tipoRender == Renderer::VectoresWiRefraccion){
		// Matriz4 base = baseFromVectorYOrigen(fig->getNormal(ptoInterseccion), ptoInterseccion, dir);
			Matriz4 base = fig->getBase(ptoInterseccion);
		bool inside = false;
		if (dir * base[2] > 0) {
			base[2] = -base[2];
			inside = true;
		}
		// Vector3 otroPath =
		// std::cout << base.inversa() * dir << std::endl;
		Material mat = fig->getMaterial();
		return mat.getVectorSalida(base, gen, 2, inside, dir);
	}
	else if (tipoRender == Renderer::VectoresWiReflexion) {
		Matriz4 base = fig->getBase(ptoInterseccion);
		return mat.getVectorSalida(base, gen, 1,false, dir);
	}
	else if (tipoRender == Renderer::VectoresWiDifusos) {
		Matriz4 base = fig->getBase(ptoInterseccion);
		return mat.getVectorSalida(base, gen, 0,false, dir);
	}
	else if (tipoRender == Renderer::Normales) {
		return fig->getNormal(ptoInterseccion);
	}
	else {
		std::cerr << "ERROR: Tipo de render desconocido" << '\n';
		exit(1);
	}
}


Color Renderer::pathTrace(const Vector3& o, const Vector3& dir, const GeneradorAleatorio& rngThread, bool inside, const bool primerRebote) const {
	// std::cout << "Trazando un path" << '\n';
	Color c = COLOR_FONDO;
	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccionFigura;
	// std::cout << "Voy a intersectar" << '\n';
	if (!usarBVH) { // Sin bvh
		interseccionFigura = e.interseccion(o, dir);
	}
	else { // con bvh
		// std::cout << "Cuidado con la interseccion con bvh" << '\n';
		interseccionFigura = bvh.interseccion(o, dir);
	}
	if (interseccionFigura) { // intersecta con alguna
		if (!primerRebote) {
			// std::cout << "No soy el primer rebote Y he intersectado con algo" << '\n';
		}
		auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
		// std::cout << "antes de emisor y blabnanjasfobnabsodlnaikjnaspian: " << fig << '\n';
		if (fig->esEmisor()) { // fin de la recursión, es un emisor
			// std::cout << "a por emision" << '\n';
			// return fig->getEmision();
			c = fig->getEmision();
			if (!primerRebote) return c*1.75; // TODO: multiplicacion bestia de la iluminacion, revisar
		}
		else {
			Figura::InterseccionData iData = interseccionFigura->first;
			// std::cout << "A ver el pto" << '\n';
			// t = iData.t;
			Vector3 ptoInterseccion = iData.punto;
			if (renderSeleccionado == Materiales) { // Path trace normal
				// std::cout << "He intersectado con un no emisor" << '\n';
				c = ruletaRusa(fig, dir, ptoInterseccion, rngThread, inside, primerRebote);
			}
			else { // otro tipo de render:
				Vector3 vector = vectorTipoRender(renderSeleccionado, fig, ptoInterseccion, dir, rngThread);
				c.setFromNormalNoAbs(vector); // color del vector, cada comp en un canal rgb
			}
		}
	}
	return c;///(t*t);
}


// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void Renderer::renderPixel(Imagen& im, const Vector3& o, const int pixel) const {
		Color color(0.0,0.0,0.0);
		auto c = e.getCamara();
		int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
		GeneradorAleatorio rngThread; // generador para el thread
		bool inside=false;
		for (int i=0; i<nRayos; i++) { // cada rayo
			Vector3 dir(c->getRayoPixel(pixel)); // una direccion
			Color cPixel = pathTrace(o, dir, rngThread, true, inside); // true para que el primero siempre rebote
			color = color + cPixel;// suma de cada path / double(nRayos);
		}
		color = color / double(nRayos); // promedio
		color.clamp(1.0); // TODO: revisar
		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}





void Renderer::consumirTasks(Imagen& im, const Vector3& origen) {
	//std::cout<<"Bueno"<<std::endl;
	// int cuenta = 0;
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
		renderPixel(im, origen, pixel);
		// cuenta++;
	}
	// std::cout<<"He dibujado: " << cuenta << " pixeles\n";
}


// Barra de progreso adaptada de: https://stackoverflow.com/a/14539953
void dibujarBarraProgreso(double porcentaje, int longitudBarra, hrc::time_point t_ini, hrc::time_point t_ahora) {
	int nCaracteres = porcentaje*longitudBarra; // numero de veces que se repite el caracter de la barra
	std::string caracter = "█"; // caracter de completado

	// std::cout  << "[" << std::flush;
	for (int i=0; i<longitudBarra; i++) {
		if (i<nCaracteres) { // Completados
			std::cout << caracter;
		}
		else {
			std::cout << "░";
		}
	}
	double t_elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t_ahora - t_ini).count();
	double min_restantes = 10;
	if (porcentaje>0) {
		min_restantes = (1.0-porcentaje)/porcentaje * t_elapsed / 60.0;
	}
	std::cout << " " << int(porcentaje*100) << '%'<< " (quedan unos " << min_restantes << " min)          \r" << std::flush;
}


void Renderer::progressBar(const int nPixeles) {
	hrc::time_point t1, t2;
	t1 = hrc::now();
	int longitudBarra = 50;
	while (true) {
		int nRestantes;
		{ //Lock
			// Las llaves son para que la guarda solo este entre ellas (scope):
			std::lock_guard<std::mutex> guarda(mtx); // asegura la SC
			if (tasks.empty()) { // Fin cuando no quedan tasks
				break;
			}
			nRestantes = tasks.size();
		} // unlock
		int nTerminados = nPixeles - nRestantes;
		double porcentaje = double(nTerminados)/double(nPixeles);
		t2 = hrc::now();
		dibujarBarraProgreso(porcentaje, longitudBarra, t1, t2);
		// dibujarBarraProgreso(porcentaje, longitudBarra);
		std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Duerme 2 s
	}
	dibujarBarraProgreso(1, longitudBarra, t1, t2);
	std::cout << std::endl;
}

void Renderer::initThreads(Imagen& im, const Vector3& origen, const int nPixeles) {
	for (int i=0; i<threads.capacity()-1; i++) {
		// std::thread t1(&Escena::consumirTasks, this, std::ref(im), std::ref(origen));
		threads.emplace_back(std::thread(&Renderer::consumirTasks, this, std::ref(im), std::ref(origen)));
	}
	threads.emplace_back(std::thread(&Renderer::progressBar, this, nPixeles));
}

void Renderer::waitThreads() {
	for (int i=0; i<threads.capacity(); i++) {
		threads[i].join();
	}
}

std::string Renderer::to_string() const {
	std::string s = "No has implementado el to_string de Renderer\n";
	return s;
}


void Renderer::render(const std::string fichero) {
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
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		tasks.push_back(pixel); // encolar cada pixel
		//renderPixel(im, o, pixel);
	}
	std::cout << "Inicializando threads... " << std::endl;
	initThreads(im, o, c->getNumPixeles()); // inicializar los threads
	// std::cout << "hecho" << '\n';
	waitThreads(); // y esperar a que terminen
	// im.setMaxFloat(1); // TODO: entender esta vaina
	// im.extendedReinhard();
	im.guardar("out/" + fichero); // guardar la imagen

	t2 = hrc::now();
	std::chrono::duration<double> t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "\nRender realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;
}



// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Renderer& e) {
	os << e.to_string();
	return os;
}


// Tests:
/* TODO: Mover los tests a un programa de tests o algo, no tiene sentido que este acoplado a esta clase
void Renderer::testBVHRender(const std::string f1, const std::string f2) {
	hrc::time_point t1, t2;
	std::string linea = "\n------------------------\n"; // para separarlo en cout
	std::cout << "Test de render secuencial...\nSin estructuras de aceleración...\n";
	t1 = hrc::now();
	testRenderMethod(Original, f1);
	t2 = hrc::now();
	std::chrono::duration<double> tiempoNormal = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "Tiempo de render original = " << tiempoNormal.count() << " segundos" << linea<<"Tiempo de construccion BVH...\n";
	t1 = hrc::now();
	bvh.construirArbol(figuras);
	t2 = hrc::now();
	std::chrono::duration<double> tiempoConstruccion = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

	std::cout << "Tiempo Construccion = " << tiempoConstruccion.count() << " segundos" << linea<<"Tiempo de render BVH...\n";
	t1 = hrc::now();
	testRenderMethod(Renderer::BVH, f2);
	t2 = hrc::now();
	std::chrono::duration<double> tiempoBVH = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "Tiempo de render con BVH = " << tiempoBVH.count() <<" segundos"<<linea;
	auto bvhTotal = tiempoBVH.count()+tiempoConstruccion.count();
	std::cout << "BVH total: " <<  bvhTotal << linea << "bvhTotal/Original = "
					 	<< bvhTotal/tiempoNormal.count() << linea << "(Speedup, Original/bvhTotal = "
						<< tiempoNormal.count()/bvhTotal << ")" << linea;
}

// metodo tiene que ser 0 para Vector o 1 para bvh
void Renderer::testRenderMethod(const Renderer::Metodo metodo, const std::string fichero) const {
	// iterar para cada pixel de la camara:
		// lanzar un rayo y colorear ese pixel del color del objeto con el que intersecte
	Vector3 o = c->getPos();
	// Vector3 ultimaDir(c->getRayoPixel(0)); // DEBUG
	Imagen im(c->getPixelesY(), c->getPixelesX());
	// std::cout <<" P3\n#MAX=1\n# out/rayos.ppm\n400 400\n255";
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		// std::cout << "pixel "<<pixel<<"\n";
		if (metodo == Original) renderPixelVector(im, o, pixel);
		else renderPixel(im, o, pixel); // metodo BVH
	}
	im.guardar(fichero);
}

// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
// Itera todo el vector de figuras
void Renderer::renderPixelVector(Imagen& im, const Vector3& o, const int pixel) const {
	bool interseccion = false;
	Color color(0.0,0.0,0.0);
	int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
	for (int i=0; i<nRayos; i++) { // cada rayo
		Vector3 dir(c->getRayoPixel(pixel)); // una direccion
		double tMin = -1; // distancia a la figura mas cercana
		// color de la figura mas cercana, por defecto el fondo:
		Color eFigCercana = {0.2,0.2,0.2};
		for (auto figura : figuras) {
			auto iData = figura->interseccion(o,dir);
			// double t =
			// interseccion = t>0; // intersecta
			if (iData && tMenor(*iData, tMin)) {
				tMin = iData->t;
				eFigCercana = figura->getEmision(dir);
			}
		}
		color = color + eFigCercana / double(nRayos);
		// for (int j=0; j<3; j++) { // Se suma el color de la figura mas cercana /nRayos para hacer la media
		// 	color[j]+=eFigCercana[j]/nRayos;
		// }
	}
	im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}



// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void Renderer::renderPixelViejo(Imagen& im, const Vector3& o, const int pixel) const {
		bool interseccion = false;
		Color color(0.0,0.0,0.0);
		int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
		for (int i=0; i<nRayos; i++) { // cada rayo
			Vector3 dir(c->getRayoPixel(pixel)); // una direccion
			// color de la figura mas cercana, por defecto el fondo:
			Color eFigCercana(0.2,0.2,0.2);
			double dist=0;
			//auto distFigura = bvh.interseccion(o, dir); // distancia
			auto interseccionFigura = bvh.interseccion(o, dir); //
			if (interseccionFigura) { // intersecta con alguna
				Figura::InterseccionData iData = interseccionFigura->first;
				double t = iData.t;
				Vector3 ptoInterseccion = iData.punto;
				auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
				if (renderSeleccionado == TipoRender::Emision) {
					eFigCercana = fig->getEmision(ptoInterseccion);//o+t*dir);// se le pasa el pto de interseccion // COLOR DE FIGURA
				} else if (renderSeleccionado == TipoRender::Normales) {
					Vector3 normal = fig->getNormal(ptoInterseccion);//o+t*dir); // Normal en el pto
					eFigCercana.setFromNormal(normal); // Color para la normal
				} else if (renderSeleccionado == TipoRender::Distancia) {
					eFigCercana.setFromDistancia(t, 1, 7); // Color para la normal
				} else if (renderSeleccionado == TipoRender::Materiales) {
					// Color
					// for (int rebote = 0;rebote<MAXREBOTES; rebote++) {
					// 	Vector3 origenRebote = ptoInterseccion;
					// 	Vector3 dirRebote = fig->getRebote(ptoInterseccion);
					// 	color+=
					// }
					// eFigCercana =
					// eFigCercana.setFromDistancia(t, 1, 7); // Color para la normal
				}
			}
			color = color + eFigCercana;// suma de cada rayo / double(nRayos);
		}
		color = color / double(nRayos); // promedio
		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}
*/
