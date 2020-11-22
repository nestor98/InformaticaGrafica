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

class LuzPuntual {
	Vector3 pos;
	Color emision;

public:
	LuzPuntual();

	LuzPuntual(const Vector3& _pos, const Color& _e = Color(1,1,1));

	std::string to_string() const;

	Vector3 getPos() const;

	// Emision -------------------------
	Color getEmision() const;
	//Vector3 getDir() const;
	void setEmision(const double r, const double g, const double b);

	// void setEmision(const std::array<double, 3> _e);
	void setEmision(const Color& e);

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const LuzPuntual& c);

	std::ostream& operator<<(std::ostream& os, const std::shared_ptr<LuzPuntual> c);
