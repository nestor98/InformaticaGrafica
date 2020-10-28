#include <iostream>
#include <string>


#include "esfera.hpp"

Esfera::Esfera(const Vector3& _posicion, const float _radio) :
	Figura(_posicion), radio(_radio)
{}


std::string Esfera::to_string() const {
	return "--- Esfera:\nposicion: " + posicion.to_string() + "\nradio: " + std::to_string(radio);
}

Vector3 Esfera::getPos() const {
	return posicion;
}

float Esfera::getRadio() const {
	return radio;
}


// True sii el rayo desde <origen>, hacia <dir> intersecta con la esfera
bool Esfera::intersecta(const Vector3& origen, const Vector3& dir) const {
	return true;
}


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Esfera& c) {
	os << c.to_string();
	return os;
}
