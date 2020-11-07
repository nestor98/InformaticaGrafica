#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include "bvh.hpp"


void BoundingVolumeH::setRightBranch(std::shared_ptr<BoundingVolumeH> _r) {
	// TODO
}
void BoundingVolumeH::setLeftBranch(std::shared_ptr<BoundingVolumeH> _l){
	// TODO
}


// True sii es nodo hoja (tiene figura y no ramas)
bool BoundingVolumeH::isLeaf() const {
	return bool(figura); // True si no es null
}

// Devuelve el vector de figuras contenidas en cajas con las que intersecta el rayo
BoundingVolumeH::vectorFigs BoundingVolumeH::puedenIntersectar(const Vector3& origen, const Vector3& dir) const {
	BoundingVolumeH::vectorFigs figuras;
	if (box.interseccion(origen, dir)) { // intersecta con la caja de este nodo
		if (isLeaf()) { // Si es hoja, añadimos su figura
			figuras->emplace_back(figura);
		}
		else { // Si no, devolvemos las figuras de las ramas izq y dcha
			auto figurasRight = right->puedenIntersectar(origen, dir); // dcha
			figuras->insert(figuras->end(), figurasRight->begin(), figurasRight->end()); // append
			auto figurasLeft = left->puedenIntersectar(origen, dir); // izq
			figuras->insert(figuras->end(), figurasLeft->begin(), figurasLeft->end()); // append
		}
	}
	return figuras;
}



BoundingVolumeH::BoundingVolumeH(const std::vector<std::shared_ptr<Figura>>& figuras) : box()
{
	std::vector<std::shared_ptr<Prisma>> figBoxes; // cajas de las figuras
	for (auto& figura : figuras) {
		figBoxes.emplace_back(figura->boundingBox());
	}
	std::cout << "-------------- Cajas: -------------\n";
	for (auto& fig : figBoxes) {
		std::cout << fig->to_string() << std::endl;
	}

	// TODO: para cada figura, construir la caja, etc...
	// Heuristicas...
}


void BoundingVolumeH::setFigura(const std::shared_ptr<Figura> f)
{
	figura = f;
}

std::string BoundingVolumeH::to_string() const {
	std::string s = "Caja: " + box.to_string() + "\n";
	if (!isLeaf()) {
		s+= "Izq:\n" + left->to_string();
		s+= "Dcha:\n" + right->to_string();
	}
	else {
		s+="Figura:\n" + figura->to_string();
	}
	return s;
}

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const BoundingVolumeH& e) {
	os << e.to_string();
	return os;
}
