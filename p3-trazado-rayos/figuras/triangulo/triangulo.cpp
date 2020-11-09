#include <iostream>
#include <string>


#include "Triangulo.hpp"
#include "prisma.hpp"

Triangulo::Triangulo(const Vector3& _normal, const Vector3& _v1, const Vector3& _v2, const Vector3& _v3) :
	normal(_normal), vertices({_v1, _v2, _v3});
{}


std::string Triangulo::to_string() const {
	return ("--- Triangulo:\nVertices: " + vertices[0].to_string() +  ", " +
				vertices[1].to_string() +  ", " + vertices[2].to_string() + "\nnormal: " + normal.to_string());
}

Vector3 Triangulo::getVertice(const int i) const {
	return vertices[i];
}

Vector3 Triangulo::getNormal() const {
	return normal;
}

// True sii el rayo desde <origen>, hacia <dir> intersecta con el Triangulo
double Triangulo::interseccion(const Vector3& origen, const Vector3& dir) const {
	double d_n = dir*normal;
	if (d_n!=0) { // intersecta
		return -(getDist()+origen*normal)/d_n; // t
	}
	return 0;
}


std::shared_ptr<Prisma> Triangulo::getBoundingBox() const {
	return std::make_shared<Prisma>(Prisma()); // TODO: caja finita que contenga Triangulo infinito...??
}


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Triangulo& c) {
	os << c.to_string();
	return os;
}
