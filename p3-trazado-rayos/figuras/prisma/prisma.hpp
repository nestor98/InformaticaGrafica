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
protected:
	Vector3 posicion; // posicion de la esquina mas cercana al origen
	Vector3 tam; // tamaño xyz
	// std::array<Plano, 4> caras;
	bool esAABB; // Axis aligned BB
	// bool esInfinito;


public:
	Prisma();
	Prisma(const Vector3& _posicion, const Vector3& _tam, const bool _esAABB=false);

	// Constructor del prisma bounding box del vector de figuras
	Prisma(const std::vector<std::shared_ptr<Figura>>& bboxes);
	// Constructor del prisma bounding box del vector de figuras,
	// ptoMediano pasa a ser la mediana de los centroides de las cajas
	Prisma(const std::vector<std::shared_ptr<Figura>>& bboxes, Vector3& ptoMediano);
	virtual std::string to_string() const override;

	virtual Vector3 getPos() const;
	Vector3 getTam() const;


	// Devuelve la normal de la figura en el <pto>
	virtual Vector3 getNormal(const Vector3& pto) const override;

	bool contiene(const Vector3& p) const;

	virtual std::optional<InterseccionData> interseccion(const Vector3& origen, const Vector3& dir) const override;

	virtual bool esInfinito() const;

	// Devuelve la AABB (prisma alineado con los ejes) que envuelve a la figura
	virtual std::shared_ptr<Prisma> getBoundingBox() const override;

	std::pair<std::shared_ptr<Prisma>, std::shared_ptr<Prisma>> partirEnEje(const int eje) const;

 	// bool interseccion(const Vector3& origen, const Vector3& dir) const;

	Vector3 getCentroide() const;
	Vector3 getPtoAleatorio() const;
	Vector3 getPtoAleatorio(const GeneradorAleatorio& gen) const; // mas rapida y aleatoria


	// Devuelve el indice del eje mas grande (0, 1 o 2)
	int getMaxEje() const;

};

std::shared_ptr<Prisma> combinar(const std::shared_ptr<Prisma> p1, const std::shared_ptr<Prisma> p2);

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Prisma& c);


// Prismas que se pueden rotar
class PrismaRotable : public Prisma {
	Matriz4 base; // base del prisma
	Matriz4 baseInversa; // inversa (del prisma al mundo)

	Vector3 getVertice(const int x,const int y,const int z) const;

public:
	PrismaRotable(const Vector3& _posicion, const Vector3& _tam);

	PrismaRotable(const Matriz4& _base, const Vector3& _tam);

	Vector3 getPos() const override;

	std::string to_string() const override;


	// Devuelve la normal de la figura en el <pto>
	Vector3 getNormal(const Vector3& pto) const override;

	//	bool contiene(const Vector3& p) const override;
	virtual std::shared_ptr<Prisma> getBoundingBox() const override;




	std::optional<InterseccionData> interseccion(const Vector3& origen, const Vector3& dir) const override;

	void rotar(const Matriz4& rotacion);

};





/*********************** PRISMAS INFINITOS *****************************/
// SOLO se usan para AABBs de planos infinitos

class CajaInfinita : public Prisma {
public:
	std::optional<Figura::InterseccionData> interseccion(const Vector3& origen, const Vector3& dir) const override;

	bool esInfinito() const override;
};
