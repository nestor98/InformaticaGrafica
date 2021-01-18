#pragma once

#include <iostream>

#include <random>
#include "Vector3.hpp"

class GeneradorAleatorio {
	// Para generacion de nums aleatorios, //(de https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution)

	mutable std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
	mutable std::uniform_real_distribution<> dis;

public:
	GeneradorAleatorio();

	double rand01() const; // devuelve un nº aleatorio uniforme entre 0 y 1
	double rand(const double min, const double max) const; // aleatorio uniforme entre min y max
	int rand(const int min, const int max) const; // entero uniforme en [min, max-1]
	std::string to_string() const;

	// Vector normal aleatorio mal distribuido:
	Vector3 vectorNormalAleatorioBruto() const;

	// Vector normal aleatorio uniforme:
	Vector3 vectorNormalAleatorio() const;


};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const GeneradorAleatorio& c);
