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
	Vector3 front;
	Vector3 left;
	Vector3 up;

	int pixelesX, pixelesY;

public:
	Camara(const Vector3& _posicion, const Vector3& _front, const Vector3& _left, const Vector3& _up,
		const int _pixelesX = 100, const int _pixelesY = 100);
	std::string to_string() const;
	Vector3 getPos() const;
	Vector3 getFront() const;
	Vector3 getLeft() const;
	Vector3 getUp() const;
	int getPixelesX() const;
	int getPixelesY() const;

	Vector3 getRayoPixel(const int i) const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Camara& c);
