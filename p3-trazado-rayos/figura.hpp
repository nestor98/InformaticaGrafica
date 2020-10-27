#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "../p1-geometria/Vector3.hpp"


class Figura {
protected:
	Vector3 posicion;
public:
	Figura(const Vector3& _posicion);
	virtual std::string to_string() const;

	Vector3 getPos() const;
	//Vector3 getDir() const;

	//virtual float evaluarRayo() const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Figura& c);
