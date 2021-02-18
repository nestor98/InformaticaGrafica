#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include <chrono> // tests

#include "ray-marcher.hpp"

#define hrc std::chrono::high_resolution_clock


RayMarcher::RayMarcher(const Escena& _e, const int _nThreads, const RayMarcher::TipoRender tipo, const bool _usarBVH, const float _rangoDinamico)
: usarBVH(_usarBVH), e(_e), renderSeleccionado(tipo), rangoDinamico(_rangoDinamico)//, threads(_nThreads)
{
	threads.reserve(_nThreads+1); // +1 por la barra de progreso
}


Color RayMarcher::pathTrace(const Vector3& o, const Vector3& dir, const GeneradorAleatorio& rngThread, const bool primerRebote) const {
	// std::cout << "Trazando un path" << '\n';
	Color c = COLOR_FONDO;
	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccionFigura;
	// std::cout << "Voy a intersectar" << '\n';
	interseccionFigura = e.interseccion(o, dir);

	if (interseccionFigura) { // intersecta con alguna
		if (!primerRebote) {
			// std::cout << "No soy el primer rebote Y he intersectado con algo" << '\n';
		}
		auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
		// std::cout << "antes de emisor y blabnanjasfobnabsodlnaikjnaspian: " << fig << '\n';
		if (fig->esEmisor()) { // fin de la recursión, es un emisor
			// std::cout << "a por emision" << '\n';
			// return fig->getEmision();
			// if (fig->)
			c = fig->getEmision(interseccionFigura->first.punto);
			// if (!primerRebote) return c*2.0; // TODO: multiplicacion bestia de la iluminacion, revisar
		}
		else {
			Figura::InterseccionData iData = interseccionFigura->first;
			// std::cout << "A ver el pto" << '\n';
			// t = iData.t;
			Vector3 ptoInterseccion = iData.punto;
			if (renderSeleccionado == Materiales) { // Path trace normal
				// std::cout << "He intersectado con un no emisor" << '\n';
				c = ruletaRusa(fig, dir, ptoInterseccion, rngThread, primerRebote);
			}
			else if (renderSeleccionado == krFresnel) {
				double kr = fig->getMaterial().krFresnel(dir, fig->getNormal(ptoInterseccion), 1.45);
				c = Color(kr); // ya va de 0 a 1
			}
			else { // otro tipo de render:
				Vector3 vector = vectorTipoRender(renderSeleccionado, fig,  dir, ptoInterseccion,rngThread);
				c.setFromNormalVector(vector); // color del vector, cada comp en un canal rgb
			}
		}
	}
	return c;///(t*t);
}


Color RayMarcher::rayMarch(const Vector3& o, const Vector3& dir, const GeneradorAleatorio& rngThread, const bool primerRebote) const {
	// std::cout << "Trazando un path" << '\n';
	Color c = COLOR_FONDO;
	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccionFigura;
	// std::cout << "Voy a intersectar" << '\n';
	interseccionFigura = e.interseccion(o, dir);

	if (interseccionFigura) { // intersecta con alguna
		auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
		// std::cout << "antes de emisor y blabnanjasfobnabsodlnaikjnaspian: " << fig << '\n';
		if (fig->esEmisor()) { // fin de la recursión, es un emisor
			// std::cout << "a por emision" << '\n';
			// return fig->getEmision();
			// if (fig->)
			c = fig->getEmision(interseccionFigura->first.punto);
		}
		else {
			Figura::InterseccionData iData = interseccionFigura->first;
			// std::cout << "A ver el pto" << '\n';
			// t = iData.t;
			Vector3 ptoInterseccion = iData.punto;
			if (renderSeleccionado == Materiales) { // Path trace normal
				// std::cout << "He intersectado con un no emisor" << '\n';
				c = ruletaRusa(fig, dir, ptoInterseccion, rngThread, primerRebote);
			}
			else if (renderSeleccionado == krFresnel) {
				double kr = fig->getMaterial().krFresnel(dir, fig->getNormal(ptoInterseccion), 1.45);
				c = Color(kr); // ya va de 0 a 1
			}
			else { // otro tipo de render:
				Vector3 vector = vectorTipoRender(renderSeleccionado, fig,  dir, ptoInterseccion,rngThread);
				c.setFromNormalVector(vector); // color del vector, cada comp en un canal rgb
			}
		}
	}
	return c;///(t*t);
}



// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void RayMarcher::renderPixel(Imagen& im, const Vector3& o, const int pixel,
const GeneradorAleatorio& rngThread) const {
		Color color(0.0,0.0,0.0);
		auto c = e.getCamara();
		int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
		for (int i=0; i<nRayos; i++) { // cada rayo
			Vector3 dir(c->getRayoPixel(pixel, rngThread)); // una direccion
			Color cPixel = rayMarch(o, dir, rngThread, true); // true para que el primero siempre rebote
			color = color + cPixel;// suma de cada path / double(nRayos);
		}
		color = color / double(nRayos); // promedio
		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}

// TODO: dividir en regiones en vez de pixeles!!!
void RayMarcher::consumirTasks(Imagen& im, const Vector3& origen) {
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
		renderPixel(im, origen, pixel,rngThread);
		// cuenta++;
	}
	// std::cout<<"He dibujado: " << cuenta << " pixeles\n";
}

void RayMarcher::initThreads(Imagen& im, const Vector3& origen, const int nPixeles) {
	for (int i=0; i<threads.capacity()-1; i++) {
		// std::thread t1(&Escena::consumirTasks, this, std::ref(im), std::ref(origen));
		threads.emplace_back(std::thread(&RayMarcher::consumirTasks, this, std::ref(im), std::ref(origen)));
	}
	threads.emplace_back(std::thread(&Renderer::progressBar, this, nPixeles));
}

void RayMarcher::waitThreads() {
	for (int i=0; i<threads.capacity(); i++) {
		threads[i].join();
	}
}

std::string RayMarcher::to_string() const {
	std::string s = "No has implementado el to_string de RayMarcher\n";
	return s;
}


void RayMarcher::render(const std::string fichero) {
	hrc::time_point t1, t2;
	t1 = hrc::now();
	auto c = e.getCamara();
	Vector3 o = c->getPos();
	Imagen im(c->getPixelesY(), c->getPixelesX());
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		tasks.push_back(pixel); // encolar cada pixel
		//renderPixel(im, o, pixel); // Secuencial de momento
	}
	// std::cout << "Inicializando threads... " << std::endl;
	// initThreads(im, o, c->getNumPixeles()); // inicializar los threads
	// // std::cout << "hecho" << '\n';
	// waitThreads(); // y esperar a que terminen
	im.setMaxFloat(rangoDinamico);
	//im.guardar("out/HDR" + fichero, true); // guardar la imagen
	im.extendedReinhard();
	im.guardar("out/" + fichero, true); // guardar la imagen

	t2 = hrc::now();
	std::chrono::duration<double> t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "\nRender realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;
}



// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const RayMarcher& e) {
	os << e.to_string();
	return os;
}
