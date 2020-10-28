#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include "escena.hpp"
#include "Imagen.hpp"


Escena::Escena(const Camara& _c) : c(_c.getPos(), _c.getFront(),_c.getLeft(),_c.getUp())
{
	//std::cout << "Constructor de escena: " << c.to_string() << std::endl;
}

void Escena::addFigura(const std::shared_ptr<Figura> f)
{
	figuras.push_back(f);
}



std::string Escena::to_string() const {
	std::string s = "camara:\n" + c.to_string() + "\nfiguras:";
	for (auto f : figuras) {
		s += "\n" + f->to_string();
	}
	return s;
}


void Escena::render(const std::string fichero) const {
	// TODO: Buena suerte xd
	// iterar para cada pixel de la camara:
		// lanzar un rayo y colorear ese pixel del color del objeto con el que intersecte
	Vector3 o = c.getPos();
	Imagen im(c.getPixelesY(), c.getPixelesX());
	for (int pixel = 0; pixel<c.getPixelesX()*c.getPixelesY(); pixel++) {
		Vector3 dir(c.getRayoPixel(pixel));
		//std::cout << "dir: " << dir << std::endl << "rayoPixel: " << c.getRayoPixel(pixel) <<  std::endl;
		bool interseccion = false;
		for (auto figura : figuras) {
			interseccion = figura->intersecta(o, dir);
			if (interseccion) { // TODO: gestionar que haya varias
				std::cout << "intersecto con " << figura->to_string() << std::endl;
				std::array<double, 3> eFig = figura->getEmision(); // emision de la figura
				im.setPixel(eFig[0], eFig[1], eFig[2], pixel); // se pone el pixel de la imagen de ese color
				break;
			}
		}
		if (!interseccion) { // fondo, no ha intersectado con nada:
			im.setPixel(0.5,0.5,0.5, pixel); // negro
		}
	}
	im.guardar(fichero);
}

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Escena& e) {
	os << e.to_string();
	return os;
}
