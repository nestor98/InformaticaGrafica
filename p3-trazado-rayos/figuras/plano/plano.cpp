#include <iostream>
#include <string>


#include "Plano.hpp"

Plano::Plano(const Vector3& _posicion, const Vector3& _normal) :
	Figura(_posicion), normal(_normal)
{}


std::string Plano::to_string() const {
	return "--- Plano:\nposicion: " + posicion.to_string() + "\nnormal: " + normal.to_string();
}

Vector3 Plano::getPos() const {
	return posicion;
}

Vector3 Plano::getNormal() const {
	return normal;
}


// True sii el rayo desde <origen>, hacia <dir> intersecta con el plano
bool Plano::intersecta(const Vector3& origen, const Vector3& dir) const {
	return true;
}

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Plano& c) {
	os << c.to_string();
	return os;
}
