#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Vector3.hpp"


#include "utils.hpp"

class Camara {
	Vector3 posicion;
	Vector3 front;
	Vector3 left;
	Vector3 up;

	int pixelesX, pixelesY;
	int rayosPixel; // num de rayos que se lanzan por pixel

	Matriz4 base;

	Matriz4 baseInversa;

	GeneradorAleatorio gen; // random, etc


public:
	Camara(const Vector3& _posicion, const Vector3& objetivo, const Vector3& _up, const double fov,
		const int _pixelesX = 720, const int _pixelesY = 720*9/16, const int _rayosPixel = 4);
	Camara(const Vector3& _posicion, const Vector3& _front, const Vector3& _left, const Vector3& _up,
		const int _pixelesX = 720, const int _pixelesY = 720*9/16, const int _rayosPixel = 4);

	std::string to_string() const;
	Vector3 getPos() const;
	Vector3 getFront() const;
	Vector3 getLeft() const;
	Vector3 getUp() const;
	int getPixelesX() const;
	int getPixelesY() const;
	int getNumPixeles() const;

	void setFocalLength(const double mm);

	void setFOV(const double fov);

	void setOrigenParaTarget(const Vector3& target);
	//double distanciaParaAnchura(const double anchura) const;

	void rotarAlrededorDeTarget(const Matriz4& rotacion);


	Vector3 getRayoPixel(const int i) const;
	int getRayosPorPixel() const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Camara& c);
