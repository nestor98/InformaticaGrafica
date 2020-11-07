#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Vector3.hpp"


#include "figura.hpp"
#include "prisma.hpp"


class Esfera : public Figura {
	float radio;
	Vector3 posicion;
public:
	Esfera(const Vector3& _posicion, const float _radio);
	std::string to_string() const override;

	Vector3 getPos() const;
	float getRadio() const;

	double interseccion(const Vector3& origen, const Vector3& dir) const override;

	void setBoundingBox();
	std::shared_ptr<Prisma> getBoundingBox() const override;


};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Esfera& c);
