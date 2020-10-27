#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
//#include "../p1-geometria/Vector.hpp"

#include "camara.hpp"

Camara::Camara(const Vector3& _posicion, const Vector3& _front, const Vector3& _left, const Vector3& _up,
							 const int _pixelesX, const int _pixelesY) :
	posicion(_posicion), front(_front), left(_left), up(_up), pixelesX(_pixelesX), pixelesY(_pixelesY)
{
	// std::cout << "Constructor de camara: " << to_string() << std::endl;
	// std::cout << "param direccion: " << _direccion << std::endl << direccion<< std::endl;

}


std::string Camara::to_string() const {
	return "posicion: " + posicion.to_string() + "\ndireccion: " + front.to_string();
}

Vector3 Camara::getPos() const {
	return posicion;
}
Vector3 Camara::getFront() const {
	return front;
}
Vector3 Camara::getLeft() const {
	return left;
}
Vector3 Camara::getUp() const {
	return up;
}

int Camara::getPixelesX() const {
	return pixelesX;
}

int Camara::getPixelesY() const {
	return pixelesY;
}

// devuelve el vector del iesimo pixel de la camara
Vector3 Camara::getRayoPixel(const int i) const {
	// TODO!!!!
}


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Camara& c) {
	os << c.to_string();
	return os;
}
