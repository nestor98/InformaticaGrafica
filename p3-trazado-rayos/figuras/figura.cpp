#include <iostream>
#include <string>

#include "figura.hpp"
#include "prisma.hpp"

Figura::Figura(const std::array<double, 3> _e) : e(_e), utils()
{}

Figura::Figura() : Figura({0,0,0})
{
	setRandomColor();
}

std::string Figura::to_string() const {
	return "Figura no tiene to_string";
}

std::array<double, 3> Figura::getEmision() const {
	return e;
}


std::shared_ptr<Prisma> Figura::getBoundingBox() const {
	std::cout << "CUIDAOOOOOOOOOOOOOOOOOOOOOOOOOOOO------------------------------------------------------\n";
	return bbox;
}


void Figura::setColor(const double r, const double g, const double b) {
	e = {r,g,b};
}

void Figura::setColor(const std::array<double, 3> _e = {1,0,0}) {
	e=_e;
}

// color aleatorio
void Figura::setRandomColor() {
	//std::cout << utils.rand01()<<std::endl;
	e = {utils.rand01(), utils.rand01(), utils.rand01()};
}

// True sii el rayo desde <origen>, hacia <dir> intersecta con la esfera
double Figura::interseccion(const Vector3& origen, const Vector3& dir) const {
	return 0;
}
// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Figura& c) {
	os << c.to_string();
	return os;
}
// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Figura> c) {
	os << c->to_string();
	return os;
}

// TODO: de momento devuelve el centroide de su bounding box, que no tiene
// pq ser el mismo que el suyo propio, ya veremos si hay que cambiarlo
Vector3 Figura::getCentroide() const {
	return getBoundingBox()->getCentroide();
}
