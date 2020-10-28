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
public:
	Plano(const Vector3& _posicion, const Vector3& _normal);
	std::string to_string() const override;

	Vector3 getPos() const;
	Vector3 getNormal() const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Plano& c);
