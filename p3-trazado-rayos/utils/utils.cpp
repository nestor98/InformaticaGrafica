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

// Vector3 GeneradorAleatorio::vectorNormalAleatorio() const {
// 	double rand1 = gen.rand01();
// 	double rand2 = gen.rand01();
// 	double theta = acos(sqrt(1.0-rand01()));
// 	double phi = 2.0 * PI * rand2;
// 	// std::cout << "---------------------------------" << '\n';
// 	// std::cout << "theta: " << theta << "\nphi: " <<phi << '\n';
// 	// std::cout << "base: " << base << '\n';
// 	wi = base * Vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta), false);
// 	return normalizar(Vector3(rand01()-0.5, rand01()-0.5, rand01()-0.5, false));
// }


Vector3 GeneradorAleatorio::vectorNormalAleatorio() const {
	return normalizar(Vector3(rand01()-0.5, rand01()-0.5, rand01()-0.5, false));
}
