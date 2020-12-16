#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Vector3.hpp"


#include "figura.hpp"
#include "prisma.hpp"

// El centro del circulo inferior esta en el origen de las coords locales
class Cilindro : public Figura {
	Matriz4 base, baseInversa; // bases mundo->local y local->mundo

	float radio, altura;

public:
	Cilindro(const Vector3& _posicion, const float _radio);
	Cilindro(const Vector3& _posicion, const float _radio,std::shared_ptr<Textura> _tex);

	std::string to_string() const override;

	Vector3 getPos() const;
	float getRadio() const;

	std::optional<InterseccionData> interseccion(const Vector3& origen, const Vector3& dir) const override;
	void addTextura(Textura _tex);
	void setBoundingBox();
	std::shared_ptr<Prisma> getBoundingBox() const override;
	//double Interseccion(const Vector3& origen, const Vector3& dir, double *distancia);



	Vector3 getNormal(const Vector3& pto) const override;

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Cilindro& c);
