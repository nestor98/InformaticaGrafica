#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <memory>
// cmath para sqrt
#include "Vector3.hpp"
#include "textura.hpp"

//#include "prisma.hpp"

#include "utils.hpp"
#include "color.hpp"


class Material {
protected:
  // Tuplas RGB de cada tipo:
	Color difuso;// Lambertian diffuse BRDF.
  Color reflexion; // • A perfect specular reflectance according to the law of reflection (delta BRDF).
  Color refraccion;// • A perfect refraction according to Snell’s law (delta BTDF)

	std::shared_ptr<Prisma> bbox;

	std::shared_ptr<Textura> tex;
	bool textura;



public:
	Material();
	Material(const std::array<double, 3> _e);
	Material(std::shared_ptr<Textura> _tex);
	virtual std::string to_string() const;

	Vector3 getPos() const;
	Color getEmision(const Vector3& dir) const;
	//Vector3 getDir() const;

	void setColor(const double r, const double g, const double b);

	void setColor(const std::array<double, 3> _e);
	void setColor(Color e);
	void setRandomColor();

	virtual double interseccion(const Vector3& origen, const Vector3& dir) const;

	// Devuelve la AABB (prisma alineado con los ejes) que envuelve a la Material
	virtual std::shared_ptr<Prisma> getBoundingBox() const;


	Vector3 getCentroide() const;

	// Le cambia el color en funcion de la posicion
	void setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max);

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Material& c);

	std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Material> c);
