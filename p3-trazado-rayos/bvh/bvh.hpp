#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "figura.hpp"
#include "prisma.hpp"

#include "utils.hpp"

// Arbol binario con las figuras
class BoundingVolumeH {
	typedef std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectorFigs;

	std::shared_ptr<Prisma> box; // caja del nodo actual

	std::shared_ptr<BoundingVolumeH> left; // Rama dcha
	std::shared_ptr<BoundingVolumeH> right; // Rama izq

	std::shared_ptr<Figura> figura; // Figura
	// ---------------------------
public:
	BoundingVolumeH();
	// Constructor de nodo hoja:
	BoundingVolumeH(std::shared_ptr<Figura> fig);
	BoundingVolumeH(const std::vector<std::shared_ptr<Figura>>& figuras);
	BoundingVolumeH(const std::vector<std::shared_ptr<Figura>>& figuras, const int eje);


	void construirArbol(const std::vector<std::shared_ptr<Figura>>& figuras);

	void construirArbol(const std::vector<std::shared_ptr<Figura>>& figuras, const int eje);

	void setRightBranch(std::shared_ptr<BoundingVolumeH> _r);
	void setLeftBranch(std::shared_ptr<BoundingVolumeH> _l);

	void setFigura(std::shared_ptr<Figura> figura);

	// True sii es nodo hoja (tiene figura y no ramas)
	bool isLeaf() const;

	vectorFigs puedenIntersectar(const Vector3& origen, const Vector3& dir) const;


	std::string to_string() const;

	void render(const std::string fichero);
};

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const BoundingVolumeH& e);
