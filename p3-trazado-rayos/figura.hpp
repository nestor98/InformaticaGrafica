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
	std::array<double, 3> e; // emitividad emision o como se llame: RGB

public:
	Figura(const Vector3& _posicion, const std::array<double, 3> _e = {1,1,1});
	virtual std::string to_string() const;

	Vector3 getPos() const;
	std::array<double, 3> getEmision() const;
	//Vector3 getDir() const;

	virtual bool intersecta(const Vector3& origen, const Vector3& dir) const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Figura& c);
