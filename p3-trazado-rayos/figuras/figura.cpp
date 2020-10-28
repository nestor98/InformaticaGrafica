#include <iostream>
#include <string>

#include "figura.hpp"

Figura::Figura(const Vector3& _posicion, const std::array<double, 3> _e) :
	posicion(_posicion), e(_e)
{}


std::string Figura::to_string() const {
	return "posicion: " + posicion.to_string();
}

Vector3 Figura::getPos() const {
	return posicion;
}

std::array<double, 3> Figura::getEmision() const {
	return e;
}

// True sii el rayo desde <origen>, hacia <dir> intersecta con la esfera
bool Figura::intersecta(const Vector3& origen, const Vector3& dir) const {
	return true;
}
// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Figura& c) {
	os << c.to_string();
	return os;
}
