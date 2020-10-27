#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "../p1-geometria/Vector3.hpp"


class Camara {
	Vector3 posicion;
	Vector3 direccion;
public:
	Camara(const Vector3& _posicion, const Vector3& _direccion);
	std::string to_string() const;

	Vector3 getPos() const;
	Vector3 getDir() const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Camara& c);
