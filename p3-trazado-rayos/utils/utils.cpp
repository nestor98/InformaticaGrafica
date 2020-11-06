#include "utils.hpp"

std::random_device rd;  //Will be used to obtain a seed for the random number engine

// inicializa el generador de nºs aleatorios
Utils::Utils() : gen(rd()), dis(0.0,1.0)
{}

// devuelve un nº aleatorio uniforme entre 0 y 1
double Utils::rand01() const {
	return dis(gen);
}
