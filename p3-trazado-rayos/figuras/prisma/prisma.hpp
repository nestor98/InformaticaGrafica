#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Vector3.hpp"


#include "figura.hpp"
#include "plano.hpp"

class Prisma : public Figura {
	Vector3 tam; // tamaño xyz
	Vector3 posicion; // posicion de la esquina mas cercana al origen
	// std::array<Plano, 4> caras;
public:
	Prisma(const Vector3& _posicion, const Vector3& _tam);
	std::string to_string() const override;

	Vector3 getPos() const;
	Vector3 getTam() const;

	bool contiene(const Vector3& p) const;

	double interseccion(const Vector3& origen, const Vector3& dir) const override;

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Prisma& c);
