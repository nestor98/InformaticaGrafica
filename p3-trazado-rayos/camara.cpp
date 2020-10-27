#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
//#include "../p1-geometria/Vector.hpp"

#include "camara.hpp"

Camara::Camara(const Vector3& _posicion, const Vector3& _direccion) :
	posicion(_posicion), direccion(_direccion)
{}


std::string Camara::to_string() const {
	return "posicion: " + posicion.to_string() + "\ndireccion: " + direccion.to_string();
}

Vector3 Camara::getPos() const {
	return posicion;
}
Vector3 Camara::getDir() const {
	return direccion;
}

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Camara& c) {
	os << c.to_string();
	return os;
}
