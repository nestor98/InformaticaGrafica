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

	float coeficienteRefraccion;


	// inline static Utils utils;
	bool coeficientesCorrectos() const;

public:
	enum Tipo {Plastico, Metal, Difuso, Especular};
	Material(const float& coef=1.45);
	Material(const Color& c1, const Color& c2, const Color& c3, const float& coef=1.45);
	//Material(bool aleatorio = false);
	Material(const Tipo predeterminado, const float& coef=1.45);
	Material(std::shared_ptr<Textura> _tex, const float& coef=1.45);
	virtual std::string to_string() const;

	Vector3 getPos() const;
	//Vector3 getDir() const;

	std::array<Color, 3> getCoeficientes() const;

	void setCoeficiente(const Color& coef, const int i);

	Color getCoeficiente(const int i) const;

	void setCoeficientes(const std::array<Color, 3>& _coefs);

	void setRandom();

	void setMaximos();
	Vector3 getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen, const int evento, const Vector3& incidente=Vector3()) const ;

	Vector3 getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen, const int evento, const Vector3& incidente, float& kr) const ;

	// devuelve un num de 0 a 4 (para difuso, especular, refraccion o evento nulo, respectivamente)
	// con probabilidades en funcion del coeficiente máximo de cada uno
	int ruletaRusa(const GeneradorAleatorio& gen, const bool primerRebote=false) const;

	double krFresnel(const Vector3& normal, const Vector3& wo, const double coefRefraccion) const;

	float getPDF(const int evento, const bool primerRebote=false) const;

	// delta si el evento es reflexion o refraccion
	bool esDelta(const int evento) const;

	//
	// // Le cambia el color en funcion de la posicion
	// void setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max);

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Material& c);

	std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Material> c);


	// ---------------------- Prefabricados:
	const Material DIFUSO_VERDE = Material(Color(0.05,0.9,0.05), Color(), Color());
	const Material DIFUSO_VERDE_MAJO = Material(Color(0,143.0/255.0,57.0/255.0), Color(), Color());
	const Material DIFUSO_ROJO = Material(Color(0.9,0.05,0.05), Color(), Color());
	const Material DIFUSO_AZUL = Material(Color(0.05,0.05,0.9), Color(), Color());
	const Material DIFUSO_GRIS = Material(Color(0.70001,0.7001,0.70001), Color(), Color());
	const Material DIFUSO_BLANCO = Material(Color(0.9,0.9,0.9), Color(), Color());
	const Material DIFUSO_MARRON = Material(Color(101.0/255.0,67/255.0,33/255.0), Color(), Color());
	const Material DIFUSO_MARRON_OSCURO = Material(Color(67.0/255.0, 26.0/255.0,34.0/255.0), Color(), Color());

	const Material PLASTICO_GRIS = Material(Color(0.45,0.45,0.45), Color(0.45,0.45,0.45), Color());
	const Material PLASTICO_DORADO = Material(Color(0.6,0.6,0.1), Color(0.3,0.3,0.3), Color());
	const Material ESPEJO = Material(Color(), Color(0.9,0.9,0.9), Color());


	const Material VIDRIO = Material(Color(), Color(), Color(0.9,0.9,0.9));
