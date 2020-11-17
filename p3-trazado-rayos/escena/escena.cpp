#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include <chrono> // tests

#include "escena.hpp"


#define hrc std::chrono::high_resolution_clock

Escena::Escena(const std::shared_ptr<Camara> _c, const int _nThreads, const Escena::TipoRender tipo)
: c(_c), renderSeleccionado(tipo)//, threads(_nThreads)
{
	threads.reserve(_nThreads);
	//std::cout << "Constructor de escena: " << c->to_string() << std::endl;
	//c = std::make_shared<Camara> _c;
}

void Escena::addFigura(const std::shared_ptr<Figura> f)
{
	figuras.push_back(f);
}

void Escena::addFiguras(const std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectFiguras) {
	figuras.insert(figuras.end(), vectFiguras->begin(), vectFiguras->end());
}


void Escena::consumirTasks(Imagen& im, const Vector3& origen) {
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

void Escena::initThreads(Imagen& im, const Vector3& origen) {
	for (int i=0; i<threads.capacity(); i++) {
		// std::thread t1(&Escena::consumirTasks, this, std::ref(im), std::ref(origen));
		threads.emplace_back(std::thread(&Escena::consumirTasks, this, std::ref(im), std::ref(origen)));
	}
}

void Escena::waitThreads() {
	for (int i=0; i<threads.capacity(); i++) {
		threads[i].join();
	}
}

std::string Escena::to_string() const {
	std::string s = "camara:\n" + c->to_string() + "\nfiguras:";
	if (figuras.size()>50) {
		s+= "Hay " + std::to_string(figuras.size()) +" figuras, no las listo todas\n";
	}
	else {
		for (auto f : figuras) {
			s += "\n" + f->to_string();
		}
	}
	s += "El arbol es:\n" + bvh.to_string();
	return s;
}


bool tMenor (const double t, const double min) {
	return (t<min || min==-1);
}

bool tMenor (const Figura::InterseccionData& iData, const double min) {
	return tMenor(iData.t, min);
}

void Escena::testBVHRender(const std::string f1, const std::string f2) {
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
	testRenderMethod(Escena::BVH, f2);
	t2 = hrc::now();
	std::chrono::duration<double> tiempoBVH = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "Tiempo de render con BVH = " << tiempoBVH.count() <<" segundos"<<linea;
	auto bvhTotal = tiempoBVH.count()+tiempoConstruccion.count();
	std::cout << "BVH total: " <<  bvhTotal << linea << "bvhTotal/Original = "
					 	<< bvhTotal/tiempoNormal.count() << linea << "(Speedup, Original/bvhTotal = "
						<< tiempoNormal.count()/bvhTotal << ")" << linea;
}

// metodo tiene que ser 0 para Vector o 1 para bvh
void Escena::testRenderMethod(const Escena::Metodo metodo, const std::string fichero) const {
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
void Escena::renderPixelVector(Imagen& im, const Vector3& o, const int pixel) const {
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

const Color COLOR_FONDO(0.2,0.2,0.2);


Color Escena::ruletaRusa(const std::shared_ptr<Figura> fig, const Vector3& pto) const {
	Material mat = fig->getMaterial();
	const int evento = mat.ruletaRusa(); // devuelve un entero entre 0 y 4 en f de las probs
	Color c;
	if (evento == 3) { // absorcion
		return c;
	}
	else { // se procesa el evento
		Matriz4 base = fig->getBase(ptoInterseccion);
		Vector3 otroPath = mat->getVectorSalida(base, gen, evento);
		c = mat->getCoeficiente(evento);
		c = c*pathTrace(ptoInterseccion, otroPath);
	}
	return c;
}

Color Escena::pathTrace(const Vector3& o, const Vector3& dir) const {
	Color c = COLOR_FONDO;
	double t = 1;
	auto interseccionFigura = bvh.interseccion(o, dir); //
	if (interseccionFigura) { // intersecta con alguna
		Figura::InterseccionData iData = interseccionFigura->first;
		t = iData.t;
		Vector3 ptoInterseccion = iData.punto;
		auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
		if (fig->emite()) {
			return fig->getEmision();
		}
		else {
			c = ruletaRusa(fig, ptoInterseccion);
		}
	}
	return c/(t*t); // TODO: revisar
}


// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void Escena::renderPixel(Imagen& im, const Vector3& o, const int pixel) const {
		bool interseccion = false;
		Color color(0.0,0.0,0.0);
		int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
		for (int i=0; i<nRayos; i++) { // cada rayo
			Vector3 dir(c->getRayoPixel(pixel)); // una direccion
			Color cPixel = pathTrace(dir, o);
			color = color + cPixel;// suma de cada rayo / double(nRayos);
		}
		color = color / double(nRayos); // promedio
		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}


// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void Escena::renderPixelViejo(Imagen& im, const Vector3& o, const int pixel) const {
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


// CODIGO viejo pa enseñar las figuras y el arbol correspondiente
// for (auto fig : figuras) {
// 	std::cout << "figura y caja: " << std::endl;
// 	std::cout << fig << std::endl;
// 	std::cout << fig->getBoundingBox()->to_string() << std::endl;
// }
// std::cout << "ESCENA\n " << this->to_string() << std::endl;
// std::cout << "----------------------------------Arbol\n" << bvh.to_string() << "\n----------------------------------\n"<< std::endl;


void Escena::render(const std::string fichero) {
	// std::cout<<"a construir el arbol\n";
	bvh.construirArbol(figuras);
		// std::cout<<"hecho\n";
		//
		// std::cout << "----------------------------------Arbol\n" << bvh.to_string() << "\n----------------------------------\n"<< std::endl;

	// std::cout << bvh << std::endl;
	// iterar para cada pixel de la camara:
		// lanzar un rayo y colorear ese pixel del color del objeto con el que intersecte
	Vector3 o = c->getPos();
	// Vector3 ultimaDir(c->getRayoPixel(0)); // DEBUG
	Imagen im(c->getPixelesY(), c->getPixelesX());
	// std::cout <<" P3\n#MAX=1\n# out/rayos.ppm\n400 400\n255";
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		tasks.push_back(pixel); // encolar cada pixel
		//renderPixel(im, o, pixel);
	}
	initThreads(im, o); // inicializar los threads
	waitThreads(); // y esperar a que terminen
	//im.extendedReinhard();
	im.guardar(fichero); // guardar la imagen

}

/* Implementacion secuencial:
void Escena::renderSecuencial(const std::string fichero) const {
	// iterar para cada pixel de la camara:
		// lanzar un rayo y colorear ese pixel del color del objeto con el que intersecte
	Vector3 o = c->getPos();
	// Vector3 ultimaDir(c->getRayoPixel(0)); // DEBUG
	Imagen im(c->getPixelesY(), c->getPixelesX());
	// std::cout <<" P3\n#MAX=1\n# out/rayos.ppm\n400 400\n255";
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		renderPixel(im, o, pixel);
	}
	im.guardar(fichero);
}*/


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Escena& e) {
	os << e.to_string();
	return os;
}
