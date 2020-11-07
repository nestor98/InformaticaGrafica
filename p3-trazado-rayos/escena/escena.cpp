#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include "escena.hpp"


Escena::Escena(const std::shared_ptr<Camara> _c, const int _nThreads)
: c(_c), utils()//, threads(_nThreads)
{
	threads.reserve(_nThreads);
	//std::cout << "Constructor de escena: " << c->to_string() << std::endl;
	//c = std::make_shared<Camara> _c;
}

void Escena::addFigura(const std::shared_ptr<Figura> f)
{
	figuras.push_back(f);
}

void Escena::consumirTasks(Imagen& im, const Vector3& origen) {
	std::cout<<"Bueno"<<std::endl;
	bool fin = false;
	int cuenta = 0;
	while (true) {
		//Lock
		if (tasks.empty()) {
			break;
		}
		int pixel = tasks.back();
		tasks.pop_back();
		// unlock
		renderPixel(im, origen, pixel);
		cuenta++;
	}
	std::cout<<"He hecho: " << cuenta << " pixeles\n";
}

void Escena::initThreads(Imagen& im, const Vector3& origen) {
	//std::cout<<"hm???????\n" << threads.capacity();
	for (int i=0; i<threads.capacity(); i++) {
		// std::thread t1(&Escena::consumirTasks, this, std::ref(im), std::ref(origen));
		//std::cout<<"hm\n";
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
	for (auto f : figuras) {
		s += "\n" + f->to_string();
	}
	return s;
}

bool tMenor (const double t, const double min) {
	return (t<min || min==-1);
}


// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void Escena::renderPixel(Imagen& im, const Vector3& o, const int pixel) const {
	bool interseccion = false;
	std::array<double, 3> color = {0.0,0.0,0.0};
	int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
	for (int i=0; i<nRayos; i++) { // cada rayo
		Vector3 dir(c->getRayoPixel(pixel)); // una direccion
		double tMin = -1; // distancia a la figura mas cercana
		// color de la figura mas cercana, por defecto el fondo:
		std::array<double, 3> eFigCercana = {0.2/nRayos,0.2/nRayos,0.2/nRayos};
		for (auto figura : figuras) {
			double t = figura->interseccion(o, dir); // distancia
			interseccion = t>0; // intersecta
						if (interseccion && tMenor(t, tMin)) {
					tMin = t;
					eFigCercana = figura->getEmision();
			}
		}
		for (int j=0; j<3; j++) { // Se suma el color de la figura mas cercana /nRayos para hacer la media
			color[j]+=eFigCercana[j]/nRayos;
		}
	}
	im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}

void Escena::render(const std::string fichero) {
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
