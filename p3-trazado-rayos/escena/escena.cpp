#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include "escena.hpp"
#include "Imagen.hpp"


Escena::Escena(const std::shared_ptr<Camara> _c) : c(_c), utils()//_c->getPos(), _c->getFront(),_c->getLeft(),_c->getUp(),_c->getPixelesX(),_c->getPixelesY(),_c->getRayosPorPixel())
{
	//std::cout << "Constructor de escena: " << c->to_string() << std::endl;
	//c = std::make_shared<Camara> _c;
}

void Escena::addFigura(const std::shared_ptr<Figura> f)
{
	figuras.push_back(f);
}



std::string Escena::to_string() const {
	std::string s = "camara:\n" + c->to_string() + "\nfiguras:";
	for (auto f : figuras) {
		s += "\n" + f->to_string();
	}
	return s;
}


void Escena::render(const std::string fichero) const {
	// iterar para cada pixel de la camara:
		// lanzar un rayo y colorear ese pixel del color del objeto con el que intersecte
	Vector3 o = c->getPos();
	// Vector3 ultimaDir(c->getRayoPixel(0)); // DEBUG
	Imagen im(c->getPixelesY(), c->getPixelesX());
	// std::cout <<" P3\n#MAX=1\n# out/rayos.ppm\n400 400\n255";
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		bool interseccion = false;
		std::array<double, 3> color = {0.0,0.0,0.0};
		for (int i=0; i<c->getRayosPorPixel(); i++) {
			Vector3 dir(c->getRayoPixel(pixel));
			for (auto figura : figuras) {
				interseccion = figura->interseccion(o, dir)>0;
				if (interseccion) { // TODO: gestionar que haya varias
					//std::cout << "intersecto con " << figura->to_string() << std::endl;
					std::array<double, 3> eFig = figura->getEmision(); // emision de la figura
					for (int j=0; j<3; j++) {
						color[j]+=eFig[j]/c->getRayosPorPixel();
					}
					//im.setPixel(dir[0], dir[1], dir[2], pixel);
					break;
				}
			}
			if (!interseccion) { // fondo, no ha intersectado con nada:
				// std::cout << "FONDO!!\n";
				double fondo = 0.5/c->getRayosPorPixel();
				for (int j=0; j<3; j++) {
					color[j]+=fondo;//*utils.rand01();
				} // fondo gris
			}
		}
		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
	}
	im.guardar(fichero);
}

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Escena& e) {
	os << e.to_string();
	return os;
}
