#include <iostream>
#include <string>
#include <math.h>

#include "Prisma.hpp"

Prisma::Prisma(const Vector3& _posicion, const Vector3& _tam) :
	posicion(_posicion), tam(_tam)//, caras[0]()
{}


std::string Prisma::to_string() const {
	return "--- Prisma:\nposicion: " + posicion.to_string() + "\ntam: " + (tam).to_string();
}

Vector3 Prisma::getPos() const {
	return posicion;
}

Vector3 Prisma::getTam() const {
	return tam;
}

// devuelve el max de 3 reales
double max3 (double d1, double d2, double d3) {
	//// std::cout << d1 << "\t" << d2 <<"\t" << d3 << "\n";
	double max = (d1>d2) ? d1 : d2;
	return (max>d3 && !isinf(max)) ? max : d3;
}

// devuelve el mayor componente del vector
double maxVect(const Vector3& v) {
	return max3(v[0], v[1], v[2]);
}

// Devuelve true sii el prisma contiene p
bool Prisma::contiene(const Vector3& p) const {
	// std::cout<<"p:"<<p<<"\n(p-posicion).esPositivo(): "<<(p-posicion).esPositivo()<<std::endl;
		// std::cout<<"(p-posicion): "<<(p-posicion)<<std::endl;
	// std::cout<<"(posicion+tam-p).esPositivo():"<<(posicion+tam-p).esPositivo()<<std::endl;
	// std::cout<<"posicion:"<<posicion<<std::endl;
	// std::cout<<"tam:"<<tam<<std::endl;
	return (p-posicion).esPositivo() && (posicion+tam-p).esPositivo();
}

// True sii el rayo desde <origen>, hacia <dir> intersecta con el Prisma
// adaptado de: https://developer.arm.com/documentation/100140/0302/advanced-graphics-techniques/implementing-reflections-with-a-local-cubemap/ray-box-intersection-algorithm
double Prisma::interseccion(const Vector3& origen, const Vector3& dir) const {
	Vector3 a = getPos(); // primera esquina
	Vector3 b = a + getTam(); // segunda
	Vector3 tA = dividirComponentes((a - origen), dir); // Primera esquina
	Vector3 tB = dividirComponentes((b - origen), dir); // Segunda

	// std::cout<<"dir: "<< dir << std::endl;
	// std::cout<<"1/dir: "<< 1/dir << std::endl;
	// std::cout<<"a: "<< a << std::endl;
	// std::cout<<"tA: "<< tA << std::endl;
	double t1 = maxVect(tA);
	double t2 = maxVect(tB);
	if (t1>t2) {
		t1 = t2;
	}
	// std::cout<<"origen:"<<origen<<"\nt1: "<<t1<<std::endl;
	Vector3 p = origen+t1*dir; // pto de interseccion

	//// std::cout<<"p: "<< p << std::endl;
	if (contiene(p)) {
		return t1;
	}
	else {
		return 0;
	}
}
