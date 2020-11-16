#include "utils.hpp"

std::random_device rd;  //Will be used to obtain a seed for the random number engine

// inicializa el generador de nºs aleatorios
GeneradorAleatorio::GeneradorAleatorio() : gen(rd()), dis(0.0,1.0)
{}

// devuelve un nº aleatorio uniforme entre 0 y 1
double GeneradorAleatorio::rand01() const {
	return dis(gen);
}

// devuelve un nº aleatorio uniforme entre min y max
double GeneradorAleatorio::rand(const double min, const double max) const {
	double r = rand01();
	return r*(max-min)+min;
	//return dis(gen);
}

// Devuelve un entero aleatorio entre [min..max), todos aprox. equiprobables
int GeneradorAleatorio::rand(const int min, const int max) const {
	double r = rand(double(min), double(max+1));
	if (int(r)==max) return int(r)-1; // que no devuelva max
	return int(r);
}
