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
	// Color difuso;// Lambertian diffuse BRDF.
  // Color reflexion; // • A perfect specular reflectance according to the law of reflection (delta BRDF).
  // Color refraccion;// • A perfect refraction according to Snell’s law (delta BTDF)
	std::array<Color, 3> coeficientes; // Lambertiano, especular, refraccion

	std::array<double, 3> maxCoefs; // Lambertiano, especular, refraccion


	// inline static Utils utils;
	bool coeficientesCorrectos() const;

public:
	enum Tipo {Plastico, Metal, Difuso, Especular};
	Material();
	Material(const Color& c1, const Color& c2, const Color& c3);
	//Material(bool aleatorio = false);
	Material(const Tipo predeterminado);
	Material(std::shared_ptr<Textura> _tex);
	virtual std::string to_string() const;

	Vector3 getPos() const;
	Color getEmision(const Vector3& dir) const;
	//Vector3 getDir() const;

	std::array<Color, 3> getCoeficientes() const;

	void setCoeficiente(const Color& coef, const int i);

	Color getCoeficiente(const int i) const;

	void setCoeficientes(const std::array<Color, 3>& _coefs);

	void setRandom();

	void setMaximos();

	Vector3 getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen, const int evento, const Vector3& incidente=Vector3()) const;

	// devuelve un num de 0 a 4 (para difuso, especular, refraccion o evento nulo, respectivamente)
	// con probabilidades en funcion del coeficiente máximo de cada uno
	int ruletaRusa(const GeneradorAleatorio& gen, const bool primerRebote=false) const;

	//
	// // Le cambia el color en funcion de la posicion
	// void setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max);

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Material& c);

	std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Material> c);


	// ---------------------- Prefabricados:
	const Material DIFUSO_VERDE = Material(Color(0.05,0.9,0.05), Color(), Color());
