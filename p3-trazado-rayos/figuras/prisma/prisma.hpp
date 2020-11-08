#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <utility> // pair
// cmath para sqrt
#include "Vector3.hpp"


#include "figura.hpp"
#include "plano.hpp"

// enable_shared_from_this, de https://en.cppreference.com/w/cpp/memory/enable_shared_from_this
class Prisma : public Figura {
	Vector3 tam; // tamaño xyz
	Vector3 posicion; // posicion de la esquina mas cercana al origen
	// std::array<Plano, 4> caras;
public:
	Prisma();
	Prisma(const Vector3& _posicion, const Vector3& _tam);

	// Constructor del prisma bounding box del vector de figuras
	Prisma(const std::vector<std::shared_ptr<Figura>>& bboxes);
	std::string to_string() const override;

	Vector3 getPos() const;
	Vector3 getTam() const;

	bool contiene(const Vector3& p) const;

	double interseccion(const Vector3& origen, const Vector3& dir) const override;

	// Devuelve la AABB (prisma alineado con los ejes) que envuelve a la figura
	std::shared_ptr<Prisma> getBoundingBox() const override;

	std::pair<std::shared_ptr<Prisma>, std::shared_ptr<Prisma>> partirEnEje(const int eje) const;

 	// bool interseccion(const Vector3& origen, const Vector3& dir) const;

	Vector3 getCentroide() const;

	// Devuelve el indice del eje mas grande (0, 1 o 2)
	int getMaxEje() const;

};

std::shared_ptr<Prisma> combinar(const std::shared_ptr<Prisma> p1, const std::shared_ptr<Prisma> p2);

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Prisma& c);
