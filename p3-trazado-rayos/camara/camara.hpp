#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Vector3.hpp"


#include <random>

class Camara {
	Vector3 posicion;
	Vector3 front;
	Vector3 left;
	Vector3 up;

	int pixelesX, pixelesY;
	int rayosPixel; // num de rayos que se lanzan por pixel

	Matriz4 base;

	Matriz4 baseInversa;

	// Para generacion de nums aleatorios
	mutable std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
	mutable std::uniform_real_distribution<> dis;

	double rand01() const; // devuelve un nº aleatorio uniforme entre 0 y 1

public:
	Camara(const Vector3& _posicion, const Vector3& _front, const Vector3& _left, const Vector3& _up,
		const int _pixelesX = 720, const int _pixelesY = 720*9/16, const int _rayosPixel = 4);

	std::string to_string() const;
	Vector3 getPos() const;
	Vector3 getFront() const;
	Vector3 getLeft() const;
	Vector3 getUp() const;
	int getPixelesX() const;
	int getPixelesY() const;

	Vector3 getRayoPixel(const int i) const;
	int getRayosPorPixel() const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Camara& c);
