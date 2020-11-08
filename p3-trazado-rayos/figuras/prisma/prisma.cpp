#include <iostream>
#include <string>
#include <math.h>

#include "Prisma.hpp"


Prisma::Prisma() :posicion(0,0,0,true), tam(0,0,0,true)
{}


Prisma::Prisma(const Vector3& _posicion, const Vector3& _tam) :
	posicion(_posicion), tam(_tam)//, caras[0]()
{}


// Devuelve el minimo de la coord i
float minPos(const std::vector<std::shared_ptr<Figura>>& prismas, const int i) {
	// std::cout <<"si\n";
	float min = prismas[0]->getBoundingBox()->getPos()[i];
	// std::cout <<"b\n";

	for (auto p : prismas) {
		// std::cout <<"forrrr\n";
		auto box = p->getBoundingBox();
		min = (box->getPos()[i] < min) ? box->getPos()[i] : min;
	}
	return min;
}


// Devuelve el minimo de la coord i
float maxPos2(const std::vector<std::shared_ptr<Figura>>& prismas, const int i) {
	// std::cout <<"mmmmmmmmmmmm\n";
	float max = prismas[0]->getBoundingBox()->getPos()[i] + prismas[0]->getBoundingBox()->getTam()[i];
	for (auto p : prismas) {
		// std::cout <<"qqqqqqqqqqqq\n";
		auto box = p->getBoundingBox();
		float pos2 = box->getPos()[i] + box->getTam()[i];
		max = (pos2 > max) ? pos2 : max;
	}
	return max;
}

// Constructor del prisma bounding box del vector de figuras
Prisma::Prisma(const std::vector<std::shared_ptr<Figura>>& bboxes)
: posicion(minPos(bboxes, 0), minPos(bboxes, 1), minPos(bboxes, 2), true),
tam(posicion[0]+maxPos2(bboxes, 0), posicion[1]+maxPos2(bboxes, 1), posicion[2]+maxPos2(bboxes, 2), true)
{
	// std::cout <<"mmmou3whwughuhmmmm\n";
}


std::string Prisma::to_string() const {
	return "--- Prisma:\nposicion: " + posicion.to_string() + "\ntam: " + (tam).to_string();
}

Vector3 Prisma::getPos() const {
	return posicion;
}

Vector3 Prisma::getTam() const {
	return tam;
}

// devuelve el max de 3 reales
double max3 (double d1, double d2, double d3) {
	//// std::cout << d1 << "\t" << d2 <<"\t" << d3 << "\n";
	double max = (d1>d2) ? d1 : d2;
	return (max>d3 && !isinf(max)) ? max : d3;
}

// devuelve el mayor componente del vector
double maxVect(const Vector3& v) {
	return max3(v[0], v[1], v[2]);
}

// Devuelve true sii el prisma contiene p
bool Prisma::contiene(const Vector3& p) const {
	// std::cout<<"p:"<<p<<"\n(p-posicion).esPositivo(): "<<(p-posicion).esPositivo()<<std::endl;
		// std::cout<<"(p-posicion): "<<(p-posicion)<<std::endl;
	// std::cout<<"(posicion+tam-p).esPositivo():"<<(posicion+tam-p).esPositivo()<<std::endl;
	// std::cout<<"posicion:"<<posicion<<std::endl;
	// std::cout<<"tam:"<<tam<<std::endl;
	return (p-posicion).esPositivo() && (posicion+tam-p).esPositivo();
}

// True sii el rayo desde <origen>, hacia <dir> intersecta con el Prisma
// adaptado de: https://developer.arm.com/documentation/100140/0302/advanced-graphics-techniques/implementing-reflections-with-a-local-cubemap/ray-box-intersection-algorithm
double Prisma::interseccion(const Vector3& origen, const Vector3& dir) const {
	Vector3 a = getPos(); // primera esquina
	Vector3 b = a + getTam(); // segunda
	Vector3 tA = dividirComponentes((a - origen), dir); // Primera esquina
	Vector3 tB = dividirComponentes((b - origen), dir); // Segunda

	// std::cout<<"dir: "<< dir << std::endl;
	// std::cout<<"1/dir: "<< 1/dir << std::endl;
	// std::cout<<"a: "<< a << std::endl;
	// std::cout<<"tA: "<< tA << std::endl;
	double t1 = maxVect(tA);
	double t2 = maxVect(tB);
	if (t1>t2) {
		t1 = t2;
	}
	// std::cout<<"origen:"<<origen<<"\nt1: "<<t1<<std::endl;
	Vector3 p = origen+t1*dir; // pto de interseccion

	//// std::cout<<"p: "<< p << std::endl;
	if (contiene(p)) {
		return t1;
	}
	else {
		return 0;
	}
}


// Devuelve la AABB (prisma alineado con los ejes) que envuelve a la figura
std::shared_ptr<Prisma> Prisma::getBoundingBox() const {
	// TODO: actualizar cuando se implementen prismas rotados
	Prisma box = *this;
	return std::make_shared<Prisma>(box);
}


// Devuelve el indice del eje mas grande
int Prisma::getMaxEje() const {
	int ejeMax = 0;
	float max = tam[0];
	if (tam[1]>tam[0]) {
		ejeMax = 1; max = tam[1];
	}
	if (tam[2]>max) {
		ejeMax = 2;
	}
	return ejeMax;
}

// Devuelve el centroide del prisma
Vector3 Prisma::getCentroide() const {
	return posicion + tam/2.0;
}

// Devuelve el prisma minimo que envuelve a p1 y p2
// Su primera esquina sera el minimo de cada coordenada de las primeras esquinas
// La segunda esquina, el maximo de cada coord de las segundas esquinas
std::shared_ptr<Prisma> combinar(const std::shared_ptr<Prisma> p1, const std::shared_ptr<Prisma> p2) {
	// PRisma 1
	Vector3 pos1 = p1->getPos(); // esquina 1
	Vector3 pos1_2 = pos1 + p1->getTam(); // esquina 2
	// Prisma 2
	Vector3 pos2 = p2->getPos(); // esq 1
	Vector3 pos2_2 = pos2 + p2->getTam(); // esq 2
	// float minx = minI(pos1, pos2, 0);
	// float miny = minI(pos1, pos2, 1);
	// float minz = minI(pos1, pos2, 2);
	Vector3 posCombinacion(minI(pos1, pos2, 0),minI(pos1, pos2, 1),minI(pos1, pos2, 2), true);
	Vector3 pos2Combinacion(maxI(pos1_2, pos2_2, 0),maxI(pos1_2, pos2_2, 1),maxI(pos1_2, pos2_2, 2), true);
	return std::make_shared<Prisma>(Prisma(posCombinacion, pos2Combinacion-posCombinacion));
}

std::pair<std::shared_ptr<Prisma>, std::shared_ptr<Prisma>> Prisma::partirEnEje(const int eje) const {
	Vector3 nuevoTam = tam;
	nuevoTam[eje] -= tam[eje]/2.0; // Tamaño de las dos, el mismo con un eje a la mitad
	// std::cout << tam << " .... " << nuevoTam << std::endl << tam[eje]/2.0;
	Prisma primero(posicion, nuevoTam); // Empieza en la misma esquina
	Vector3 pos2 = posicion;
	pos2[eje] += tam[eje]/2.0; // desplazado la mitad de su tam en el eje dado
	Prisma segundo(pos2, nuevoTam);
	return std::pair<std::shared_ptr<Prisma>,std::shared_ptr<Prisma>>(std::make_shared<Prisma>(primero), std::make_shared<Prisma>(segundo));
} // return primero, segundo   <----- esto es lo de arriba en python :/
