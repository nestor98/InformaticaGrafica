#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "figura.hpp"
#include "prisma.hpp"

#include "utils.hpp"

// Arbol binario con las figuras
// Implementado a partir de: http://www.pbr-book.org/3ed-2018/Primitives_and_Intersection_Acceleration/Bounding_Volume_Hierarchies.html

class BoundingVolumeH {
	typedef std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectorFigs;

	std::vector<std::shared_ptr<Figura>> vectorPlanos; // Los planos infinitos se guardan aparte para no tener cajas infinitas
	bool tieneFigsFinitas;

	std::shared_ptr<Prisma> box; // caja del nodo actual

	std::shared_ptr<BoundingVolumeH> left; // Rama dcha
	std::shared_ptr<BoundingVolumeH> right; // Rama izq

	std::shared_ptr<Figura> figura; // Figura

	std::vector<std::shared_ptr<Figura>> masFiguras; // Casos particulares

	GeneradorAleatorio gen;
	// ---------------------------

	// Auxiliares de construirArbol e interseccion
	void construirArbolRec(std::vector<std::shared_ptr<Figura>>& figuras);
	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccionFinitas(const Vector3& origen, const Vector3& dir) const;
	std::vector<std::shared_ptr<Figura> >::iterator casoParticular(std::vector<std::shared_ptr<Figura>>& figuras, const std::shared_ptr<Prisma> box, std::vector<std::shared_ptr<Figura>>::iterator it);

public:
	BoundingVolumeH();
	// Constructor de nodo hoja:
	BoundingVolumeH(std::shared_ptr<Figura> fig);
	BoundingVolumeH(std::vector<std::shared_ptr<Figura>>& figuras);


	void construirArbol(std::vector<std::shared_ptr<Figura>>& figuras);

	void setRightBranch(std::shared_ptr<BoundingVolumeH> _r);
	void setLeftBranch(std::shared_ptr<BoundingVolumeH> _l);

	void setFigura(std::shared_ptr<Figura> figura);

	// True sii es nodo hoja (tiene figura y no ramas)
	bool isLeaf() const;


	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccion(const Vector3& origen, const Vector3& dir) const;


	//vectorFigs puedenIntersectar(const Vector3& origen, const Vector3& dir) const;


	std::string to_string(const std::string prefijo="") const;

	void render(const std::string fichero);
};

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const BoundingVolumeH& e);
