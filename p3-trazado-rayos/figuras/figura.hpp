#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Vector3.hpp"


class Figura {
protected:
	std::array<double, 3> e; // emitividad emision o como se llame: RGB

public:
	Figura(const std::array<double, 3> _e = {1,0,0});
	virtual std::string to_string() const;

	Vector3 getPos() const;
	std::array<double, 3> getEmision() const;
	//Vector3 getDir() const;

	void setColor(const double r, const double g, const double b);

	void setColor(const std::array<double, 3> _e);

	virtual double interseccion(const Vector3& origen, const Vector3& dir) const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Figura& c);
