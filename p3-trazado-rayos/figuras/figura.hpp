#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <memory>
// cmath para sqrt
#include "Vector3.hpp"
//#include "prisma.hpp"

#include "utils.hpp"

class Prisma;

class Figura {
protected:
	std::array<double, 3> e; // emitividad emision o como se llame: RGB

	Utils utils; // random, etc

	std::shared_ptr<Prisma> bbox;

public:
	Figura();
	Figura(const std::array<double, 3> _e);
	virtual std::string to_string() const;

	Vector3 getPos() const;
	std::array<double, 3> getEmision() const;
	//Vector3 getDir() const;

	void setColor(const double r, const double g, const double b);

	void setColor(const std::array<double, 3> _e);

	void setRandomColor();

	virtual double interseccion(const Vector3& origen, const Vector3& dir) const;

	// Devuelve la AABB (prisma alineado con los ejes) que envuelve a la figura
	virtual std::shared_ptr<Prisma> getBoundingBox() const;

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Figura& c);

	std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Figura> c);
