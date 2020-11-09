#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Vector3.hpp"


#include "figura.hpp"


class Triangulo : public Figura {
	Vector3 normal;
	std::array<Vector3,3> vertices; // sus tres vertices

public:
	Triangulo(const Vector3& _normal, const Vector3& _v1, const Vector3& _v2, const Vector3& _v3);
	std::string to_string() const override;

	Vector3 getVertice(const int i) const;
	Vector3 getNormal() const;
	double interseccion(const Vector3& origen, const Vector3& dir) const override;
	std::shared_ptr<Prisma> getBoundingBox() const override;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Triangulo& c);
