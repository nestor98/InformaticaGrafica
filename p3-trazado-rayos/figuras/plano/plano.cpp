#include <iostream>
#include <string>


#include "plano.hpp"
#include "prisma.hpp"

Plano::Plano(const Vector3& _normal, const double _dist) :
	dist(_dist), normal(_normal)
{}


std::string Plano::to_string() const {
	return "--- Plano:\ndistancia: " + std::to_string(dist) + "\nnormal: " + normal.to_string();
}

double Plano::getDist() const {
	return dist;
}

Vector3 Plano::getNormal() const {
	return normal;
}

// True sii el rayo desde <origen>, hacia <dir> intersecta con el plano
double Plano::interseccion(const Vector3& origen, const Vector3& dir) const {
	double d_n = dir*normal;
	if (d_n!=0) { // intersecta
		return -(getDist()+origen*normal)/d_n; // t
	}
	return 0;
}


std::shared_ptr<Prisma> Plano::getBoundingBox() const {
	return std::make_shared<Prisma>(Prisma()); // TODO: caja finita que contenga plano infinito...??
}


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Plano& c) {
	os << c.to_string();
	return os;
}
