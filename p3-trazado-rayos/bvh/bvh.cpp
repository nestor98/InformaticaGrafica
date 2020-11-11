#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include <algorithm> // std::partition

#include "bvh.hpp"



BoundingVolumeH::BoundingVolumeH(std::shared_ptr<Figura> fig) : figura(fig), vectorPlanos(), tieneFigsFinitas(true) {
	// figura = fig;
	box = figura->getBoundingBox();
}

void BoundingVolumeH::construirArbol(std::vector<std::shared_ptr<Figura>>& figuras) {
	// Partimos las figuras en planos y no planos:
	auto it = std::partition(figuras.begin(), figuras.end(), [](const std::shared_ptr<Figura> f)
	{
		return f->getBoundingBox()->esInfinito();
	});
	if (it == figuras.begin()) { // No  hay planos
		vectorPlanos.clear();
		construirArbolRec(figuras);
	}
	else if (it == figuras.end()) { // Todo planos
		std::cout << "Son todos planos\n";
		tieneFigsFinitas = false;
		vectorPlanos.insert(vectorPlanos.end(), figuras.begin(), it);
	}
	else { // Figuras y planos
		std::cout << "Hay planos y figuras finitas\n";
		vectorPlanos.insert(vectorPlanos.end(), figuras.begin(), it); // Tenemos la primera mitad
		// left = std::make_shared<BoundingVolumeH>(BoundingVolumeH(mitad));
		std::vector<std::shared_ptr<Figura>> figsFinitas;
		figsFinitas.insert(figsFinitas.end(), it, figuras.end());
		construirArbolRec(figsFinitas);
	}
}
// Construccion del arbol adaptada de https://www.youtube.com/watch?v=xUszK2xNL3I
void BoundingVolumeH::construirArbolRec(std::vector<std::shared_ptr<Figura>>& figuras) {
	tieneFigsFinitas = true;
	int nFigs = figuras.size(); // num de figuras
	if (nFigs == 1) { // hoja
		figura = figuras.back();
		box = figuras[0]->getBoundingBox();
	}
	else if (nFigs == 2) { // Dos hojas
		left = std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(figuras[0]));
		right = std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(figuras[1]));
		box = combinar(figuras[0]->getBoundingBox(), figuras[1]->getBoundingBox());
	}
	else {
		box = std::shared_ptr<Prisma>(new Prisma(figuras));
		int eje = box->getMaxEje();
		float ptoCentro = box->getCentroide()[eje];
    auto it = std::partition(figuras.begin(), figuras.end(), [eje, ptoCentro](const std::shared_ptr<Figura> f){return f->getCentroide()[eje] < ptoCentro;});
		if (it == figuras.begin() || it == figuras.end()) { // TODO: problemas
			std::cout << "Oh no\n" << *it << "\n" << *figuras.begin() << "\n" << *figuras.end()<<"\n";
		}
		else { // Se ha dividido:
			std::vector<std::shared_ptr<Figura>> mitad;
			mitad.insert(mitad.end(), figuras.begin(), it); // Tenemos la primera mitad
			left = std::make_shared<BoundingVolumeH>(BoundingVolumeH(mitad));
			std::vector<std::shared_ptr<Figura>> mitadDcha;
			mitadDcha.insert(mitadDcha.end(), it, figuras.end());
			right = std::make_shared<BoundingVolumeH>(BoundingVolumeH(mitadDcha));
		}
	}
}

// True sii es nodo hoja (tiene figura y no ramas)
bool BoundingVolumeH::isLeaf() const {
	return bool(figura); // True si no es null
}



std::pair<float, std::shared_ptr<Figura>> BoundingVolumeH::interseccion(const Vector3& origen, const Vector3& dir) const {
	float tPlanosMin=0;
	std::shared_ptr<Figura> intersectado; // plano
		// std::cout << "intersectando\n";
	for (auto plano : vectorPlanos) {
		// std::cout << "intersectando plano\n";
		float tPlano = plano->interseccion(origen, dir);
		if (tPlano>0 && (tPlano < tPlanosMin || tPlanosMin==0)) {
			intersectado = plano;
			tPlanosMin = tPlano;
		}
	}
	// std::cout << "Intersectando figs\n";
	if (tieneFigsFinitas) {
			// std::cout << "intersectadas figs\n";
			auto iFinitas = interseccionFinitas(origen, dir);
			float tFinitas = iFinitas.first;
			// if (tFinitas>0) {
			//
			// 	std::cout << tFinitas << "      "<< tPlanosMin<< std::endl;
			// }
			if (tFinitas < tPlanosMin && tFinitas>0 || tPlanosMin==0) {
				return iFinitas;
			}
			//else return std::pair<float, std::shared_ptr<Figura>>(tPlanosMin, intersectado);
	} // Si llega aqui intersecta con un plano:
	return std::pair<float, std::shared_ptr<Figura>>(tPlanosMin, intersectado);

}


// Devuelve la distancia a la figura mas cercana intersectada y asigna a intersectada la figura intersectada
// Devuelve 0 si no intersecta con ninguna
std::pair<float, std::shared_ptr<Figura>> BoundingVolumeH::interseccionFinitas(const Vector3& origen, const Vector3& dir) const
{
	// std::cout << "intersectando nodo de bvh...\n";
	if (box->interseccion(origen, dir)) {
		// std::cout << "Pos no se\n";
		if (isLeaf()) {
			// if (figura->getBoundingBox()->esInfinito()) {
			// 	std::cout << figura << "\ntiene una caja infinita!!\n";
			// }
			return std::pair<float, std::shared_ptr<Figura>>(figura->interseccion(origen, dir), figura);
		}
		else { // caja padre:
			std::shared_ptr<Figura> fLeft, fRight;
			auto resLeft = left->interseccion(origen, dir); // Distancia de interseccion dcha
			auto resRight = right->interseccion(origen, dir);
			// std::cout << "intersectado algo..." << tLeft << "   ...    " << tRight<< "\n";
			float tLeft = resLeft.first;
			float tRight = resRight.first;
			if ((tLeft < tRight || tRight == 0) && tLeft > 0) {
				 // = fLeft;
				// std::cout << "intersectada a la izq: \n";
				// std::cout<<resLeft.second<< std::endl;
				return resLeft;
			}
			else if ((tRight < tLeft || tLeft == 0) && tRight > 0) {
				// std::cout << "intersectada a la dcha: " <<intersectada<< std::endl;
				return resRight;
			}
		}
	}

	// No hay interseccion:
	return 	std::pair<float, std::shared_ptr<Figura>>(0,0);
}

// 
// // Devuelve el vector de figuras contenidas en cajas con las que intersecta el rayo
// // TODO: revisar que intersecte con varias, devolver la mas cercana
// BoundingVolumeH::vectorFigs BoundingVolumeH::puedenIntersectar(const Vector3& origen, const Vector3& dir) const
// {
// 	std::cout << "hasta aqui bien\n";
// 	BoundingVolumeH::vectorFigs figuras; // puntero a vector de punteros a figuras
// 	if (box->interseccion(origen, dir)) { // intersecta con la caja de este nodo
// 		if (isLeaf()) { // Si es hoja, añadimos su figura
// 			figuras->emplace_back(figura);
// 		}
// 		else { // Si no, devolvemos las figuras de las ramas izq y dcha
//
// 				std::cout << "else..........\n";
// 			auto figurasRight = right->puedenIntersectar(origen, dir); // dcha
// 			figuras->insert(figuras->end(), figurasRight->begin(), figurasRight->end()); // append
// 			auto figurasLeft = left->puedenIntersectar(origen, dir); // izq
// 			figuras->insert(figuras->end(), figurasLeft->begin(), figurasLeft->end()); // append
// 		}
// 	}
// 	return figuras;
// }

BoundingVolumeH::BoundingVolumeH() : vectorPlanos(), tieneFigsFinitas(true)
{
}

BoundingVolumeH::BoundingVolumeH(std::vector<std::shared_ptr<Figura>>& figuras) : vectorPlanos(), tieneFigsFinitas(true)
{
	construirArbol(figuras);
}


void BoundingVolumeH::setFigura(const std::shared_ptr<Figura> f)
{
	figura = f;
}

std::string BoundingVolumeH::to_string(const std::string prefijo) const {
	// //std::cout << "\n\n---------------------------------TO_STRING\naja\n";
	std::string s = "------ BVH ------\n";
	if (!vectorPlanos.empty()) {
		s += "Planos infinitos:\n";
		for (auto plano : vectorPlanos) {
			s += plano->to_string() +"\n";
		}
	}
	if (tieneFigsFinitas) {
		s += prefijo+"Caja: " + box->to_string() + "\n";

		// std::cout << "----------------------\n" << s;
		if (!isLeaf()) {
			// std::cout << "No es hoja\n";
			s+= prefijo+"Izq:\n" + left->to_string(prefijo+"\t");
			s+= prefijo+"Dcha:\n" + right->to_string(prefijo+"\t");
		}
		else {
			s+=prefijo+"Figura:\n" + figura->to_string() + "\n";
		}

	}
	return s;
}

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const BoundingVolumeH& e) {
	os << e.to_string();
	return os;
}
