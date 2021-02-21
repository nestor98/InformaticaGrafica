#pragma once
//
// #include <iostream>
// #include <array>
// #include <string>
// #include <cmath>
// #include <vector>
// #include <memory>
// #include <optional> // TODO: cambiar todas las subclases
// // cmath para sqrt
// #include "Vector3.hpp"
// #include "textura.hpp"
// #include "material.hpp"
//
// //#include "prisma.hpp"
//
// #include "utils.hpp"
#include "color.hpp"

class Vector3;

class LuzDireccional {
	Vector3 dir;
	Color emision;

public:
	LuzDireccional();

	LuzDireccional(const Vector3& _dir, const Color& _e = Color(1,1,1));

	std::string to_string() const;

	Vector3 getDir() const;

	// Emision -------------------------
	Color getEmision() const;
	//Vector3 getDir() const;
	void setEmision(const double r, const double g, const double b);

	// void setEmision(const std::array<double, 3> _e);
	void setEmision(const Color& e);


};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const LuzDireccional& c);

	std::ostream& operator<<(std::ostream& os, const std::shared_ptr<LuzDireccional> c);
