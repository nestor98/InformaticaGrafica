#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include "bvh.hpp"



BoundingVolumeH::BoundingVolumeH(std::shared_ptr<Figura> fig) : figura(fig) {
	// figura = fig;
	//box = figura->getBoundingBox();
}

// True sii es nodo hoja (tiene figura y no ramas)
bool BoundingVolumeH::isLeaf() const {
	return bool(figura); // True si no es null
}

// Devuelve el vector de figuras contenidas en cajas con las que intersecta el rayo
BoundingVolumeH::vectorFigs BoundingVolumeH::puedenIntersectar(const Vector3& origen, const Vector3& dir) const
{
	BoundingVolumeH::vectorFigs figuras;
	if (box->interseccion(origen, dir)) { // intersecta con la caja de este nodo
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

BoundingVolumeH::BoundingVolumeH()
{
	std::cout << "Yeeee\n";

}

BoundingVolumeH::BoundingVolumeH(const std::vector<std::shared_ptr<Figura>>& figuras) //: box()
{
	construirArbol(figuras);
}


BoundingVolumeH::BoundingVolumeH(const std::vector<std::shared_ptr<Figura>>& figuras, const int eje) //: box()
{
	construirArbol(figuras, eje);
}



// Construccion del arbol adaptada de https://www.youtube.com/watch?v=xUszK2xNL3I
void BoundingVolumeH::construirArbol(const std::vector<std::shared_ptr<Figura>>& figuras) {
	// std::vector<std::shared_ptr<Prisma>> figBoxes; // cajas de las figuras
	// for (auto& figura : figuras) {
	// 	figBoxes.emplace_back(figura->getBoundingBox());
	// }
	construirArbol(figuras, 0);
}



// Construccion del arbol adaptada de https://www.youtube.com/watch?v=xUszK2xNL3I
void BoundingVolumeH::construirArbol(const std::vector<std::shared_ptr<Figura>>& figuras, const int eje) {
	int nFigs = figuras.size(); // num de figuras
	if (nFigs == 1) { // hoja
		left = (std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(figuras[0]))); // Construir la rama izq con la figura y su caja
		box = figuras[0]->getBoundingBox();
	}
	else if (nFigs == 2) { // Dos hojas
		left = std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(figuras[0]));
		// left(figuras[0]);
		right = std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(figuras[1]));
		box = combinar(figuras[0]->getBoundingBox(), figuras[1]->getBoundingBox());
	}
	else {
		box = std::shared_ptr<Prisma>(new Prisma(figuras));
		std::cout <<"CAJA: \n"<<box->to_string()<<std::endl;
		auto dosCajas = box->partirEnEje(eje); // pair de cajas al partir box en <eje>
		std::cout <<"CAJA1: \n"<<dosCajas.first->to_string()<<std::endl;
		std::cout <<"CAJA2: \n"<<dosCajas.second->to_string()<<std::endl;
		// auto listas =
		// left = std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(dosCajas.first->contiene(figuras), (eje+1)%3));
		// right = std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(dosCajas.first->contiene(figuras), (eje+1)%3)));
		// TODO: seguir http://www.pbr-book.org/3ed-2018/Primitives_and_Intersection_Acceleration/Bounding_Volume_Hierarchies.html
	}
}

void BoundingVolumeH::setFigura(const std::shared_ptr<Figura> f)
{
	figura = f;
}

std::string BoundingVolumeH::to_string() const {
	std::string s = "Caja: " + box->to_string() + "\n";
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
