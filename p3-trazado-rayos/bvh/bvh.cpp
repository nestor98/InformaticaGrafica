#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include <algorithm> // std::partition

#include "bvh.hpp"



BoundingVolumeH::BoundingVolumeH(std::shared_ptr<Figura> fig) : figura(fig) {
	// figura = fig;
	box = figura->getBoundingBox();
}

// Construccion del arbol adaptada de https://www.youtube.com/watch?v=xUszK2xNL3I
void BoundingVolumeH::construirArbol(std::vector<std::shared_ptr<Figura>>& figuras) {
	int nFigs = figuras.size(); // num de figuras

	// std::cout << "-------------------\n";
	if (nFigs == 1) { // hoja
		// left = (std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(figuras[0]))); // Construir la rama izq con la figura y su caja
		// std::cout << "creando hoja...\n";
		// box = figuras[0]->getBoundingBox();
		// std::cout << "oki, con caja:\n" << box->to_string() << "---------\n";
		// std::cout << "es hoja?:\n" << left->isLeaf() << "---------\n";
		figura = figuras.back();
		box = figuras[0]->getBoundingBox();
		// std::cout << "oki, con caja:\n" << box->to_string() << "---------\n";
		// std::cout << "es hoja?:\n" << isLeaf() << "\n\n---------\n\n";
	}
	else if (nFigs == 2) { // Dos hojas
		// std::cout << "MIRA SON DOS\n";
		left = std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(figuras[0]));
		// left(figuras[0]);
		right = std::shared_ptr<BoundingVolumeH>(new BoundingVolumeH(figuras[1]));
		box = combinar(figuras[0]->getBoundingBox(), figuras[1]->getBoundingBox());
		// std::cout << "HOJA DOBLE\n";
		// std::cout << "oki, con caja:\n" << box->to_string() << "---------\n";
	}
	else {
		// std::cout <<"CAJAS: \n";
		// for (auto f : figuras) {
		// 	std::cout << f->getBoundingBox()->to_string()<<std::endl;
		// }
		box = std::shared_ptr<Prisma>(new Prisma(figuras));
		// std::cout <<"CAJA: \n"<<box->to_string()<<std::endl;
		//auto dosCajas = box->partirEnEje(eje); // pair de cajas al partir box en <eje>
		// std::cout <<"CAJA1: \n"<<dosCajas.first->to_string()<<std::endl;
		// std::cout <<"CAJA2: \n"<<dosCajas.second->to_string()<<std::endl;
		int eje = box->getMaxEje();
		float ptoCentro = box->getCentroide()[eje];
		// std::cout << "eje: " << eje << "\nptoCentro: " << ptoCentro << std::endl;

		// for (auto f:figuras) {
		// 	std::cout <<f<<std::endl;
		// }
		// std::cout << "DESPUES\n";
    auto it = std::partition(figuras.begin(), figuras.end(), [eje, ptoCentro](const std::shared_ptr<Figura> f){return f->getCentroide()[eje] < ptoCentro;});
		if (it == figuras.begin() || it == figuras.end()) { // TODO: problemas
			std::cout << "Oh no\n" << *it << "\n" << *figuras.begin() << "\n" << *figuras.end()<<"\n";
		}
		else { // Se ha dividido:
			std::vector<std::shared_ptr<Figura>> mitad;
			// for (auto f:figuras) {
			// 	std::cout <<f<<std::endl;
			// }
			mitad.insert(mitad.end(), figuras.begin(), it); // Tenemos la primera mitad
			left = std::make_shared<BoundingVolumeH>(BoundingVolumeH(mitad));
			std::vector<std::shared_ptr<Figura>> mitadDcha;
			mitadDcha.insert(mitadDcha.end(), it, figuras.end());
			right = std::make_shared<BoundingVolumeH>(BoundingVolumeH(mitadDcha));
			// std::copy(figuras.begin(), it, mitad);
			// std::cout << "Mitad:\n";
			// for (auto f : mitad) {
			// 	std::cout << f << std::endl;
			// }
		}
	}
	// std::cout << "a ver........\n";
	// std::cout << "CAJA FIN" << box->to_string();
	// std::cout << to_string();
}

// True sii es nodo hoja (tiene figura y no ramas)
bool BoundingVolumeH::isLeaf() const {
	return bool(figura); // True si no es null
}


// Devuelve la distancia a la figura mas cercana intersectada y asigna a intersectada la figura intersectada
// Devuelve 0 si no intersecta con ninguna
	std::pair<float, std::shared_ptr<Figura>> BoundingVolumeH::interseccion(const Vector3& origen, const Vector3& dir) const
{
	// std::cout << "intersectando nodo de bvh...\n";
	if (box->interseccion(origen, dir)) {
		if (isLeaf()) {
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


// Devuelve el vector de figuras contenidas en cajas con las que intersecta el rayo
// TODO: revisar que intersecte con varias, devolver la mas cercana
BoundingVolumeH::vectorFigs BoundingVolumeH::puedenIntersectar(const Vector3& origen, const Vector3& dir) const
{
	std::cout << "hasta aqui bien\n";
	BoundingVolumeH::vectorFigs figuras; // puntero a vector de punteros a figuras
	if (box->interseccion(origen, dir)) { // intersecta con la caja de este nodo
		if (isLeaf()) { // Si es hoja, añadimos su figura
			figuras->emplace_back(figura);
		}
		else { // Si no, devolvemos las figuras de las ramas izq y dcha

				std::cout << "else..........\n";
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
}

BoundingVolumeH::BoundingVolumeH(std::vector<std::shared_ptr<Figura>>& figuras) //: box()
{
	construirArbol(figuras);
}

//
// BoundingVolumeH::BoundingVolumeH(const std::vector<std::shared_ptr<Figura>>& figuras, const int eje) //: box()
// {
// 	construirArbol(figuras, eje);
// }



// // Construccion del arbol adaptada de https://www.youtube.com/watch?v=xUszK2xNL3I
// void BoundingVolumeH::construirArbol(const std::vector<std::shared_ptr<Figura>>& figuras) {
// 	// std::vector<std::shared_ptr<Prisma>> figBoxes; // cajas de las figuras
// 	// for (auto& figura : figuras) {
// 	// 	figBoxes.emplace_back(figura->getBoundingBox());
// 	// }
// 	construirArbol(figuras, 0);
// }



void BoundingVolumeH::setFigura(const std::shared_ptr<Figura> f)
{
	figura = f;
}

std::string BoundingVolumeH::to_string(const std::string prefijo) const {
	// //std::cout << "\n\n---------------------------------TO_STRING\naja\n";
	std::string s = prefijo+"Caja: " + box->to_string() + "\n";

	// std::cout << "----------------------\n" << s;
	if (!isLeaf()) {
		// std::cout << "No es hoja\n";
		s+= prefijo+"Izq:\n" + left->to_string(prefijo+"\t");
		s+= prefijo+"Dcha:\n" + right->to_string(prefijo+"\t");
	}
	else {
		s+=prefijo+"Figura:\n" + figura->to_string() + "\n";
	}
		// std::cout << "aja2\n";
	return prefijo + s;
}

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const BoundingVolumeH& e) {
	os << e.to_string();
	return os;
}
