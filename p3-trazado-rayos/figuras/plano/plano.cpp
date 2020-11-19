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

Vector3 Plano::getNormal(const Vector3& pto) const {
	return getNormal(); // misma normal en todos los ptos
}

// True sii el rayo desde <origen>, hacia <dir> intersecta con el plano
std::optional<Figura::InterseccionData> Plano::interseccion(const Vector3& origen, const Vector3& dir) const {
	double d_n = dir*normal;
	if (d_n!=0) { // intersecta
		double t = -(getDist()+origen*normal)/d_n;
		if (t<0) return std::nullopt;
		else return InterseccionData{t, origen+dir*t}; // t
	}
	// std::cout << "Interseccion con plano\n";
	return std::nullopt;
}


std::shared_ptr<Prisma> Plano::getBoundingBox() const {
	return std::make_shared<CajaInfinita>(CajaInfinita()); // TODO: caja finita que contenga plano infinito...??
}


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Plano& c) {
	os << c.to_string();
	return os;
}
