#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <memory>
#include <optional> // TODO: cambiar todas las subclases
// cmath para sqrt
#include "Vector3.hpp"
#include "textura.hpp"

//#include "prisma.hpp"

#include "utils.hpp"
#include "color.hpp"

class Prisma;

class Figura {
// ----------------------------- Interseccion:
public:
	struct InterseccionData {
		double t; // distancia
		Vector3 punto; // TODO: en realidad no se necesita siempre siempre, pero a veces sale gratis
		//Vector3 normal; // no se necesita siempre (solo en la mas cercana), y se calcula aparte
		//std::shared_ptr<Figura> figura; // figura intersectada, no se si hace falta
		// TODO: mas cosas? Base en el punto...
	};
protected:
	Color e; // emitividad emision o como se llame: RGB


	std::shared_ptr<Prisma> bbox;

	std::shared_ptr<Textura> tex;
	bool textura;



public:
	Figura();
	Figura(const std::array<double, 3> _e);
	Figura(std::shared_ptr<Textura> _tex);
	virtual std::string to_string() const;

	Vector3 getPos() const;
	Color getEmision(const Vector3& dir) const;
	//Vector3 getDir() const;

	void setColor(const double r, const double g, const double b);

	void setColor(const std::array<double, 3> _e);
	void setColor(Color e);
	void setRandomColor();

	virtual std::optional<InterseccionData> interseccion(const Vector3& origen, const Vector3& dir) const;

	// Devuelve la normal de la figura en el <pto>
	virtual Vector3 getNormal(const Vector3& pto) const;

	// Devuelve la AABB (prisma alineado con los ejes) que envuelve a la figura
	virtual std::shared_ptr<Prisma> getBoundingBox() const;


	Vector3 getCentroide() const;

	// Le cambia el color en funcion de la posicion
	void setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max);


};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Figura& c);

	std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Figura> c);
