#include <iostream>
#include <string>

#include "figura.hpp"

Figura::Figura(const Vector3& _posicion) :
	posicion(_posicion)
{}


std::string Figura::to_string() const {
	return "posicion: " + posicion.to_string();
}

Vector3 Figura::getPos() const {
	return posicion;
}

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Figura& c) {
	os << c.to_string();
	return os;
}
