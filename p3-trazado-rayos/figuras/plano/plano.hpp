#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Vector3.hpp"


#include "figura.hpp"


class Plano : public Figura {
	Vector3 normal;
	double dist; // distancia al origen
public:
	Plano(const Vector3& _normal, const double _dist);
	std::string to_string() const override;

	double getDist() const;
	Vector3 getNormal() const;


	// Devuelve la normal de la figura en el <pto>
	Vector3 getNormal(const Vector3& pto) const override;

	std::optional<InterseccionData> interseccion(const Vector3& origen, const Vector3& dir) const override;
	std::shared_ptr<Prisma> getBoundingBox() const override;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Plano& c);
