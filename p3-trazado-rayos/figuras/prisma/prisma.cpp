#include <iostream>
#include <string>
#include <math.h>

#include "prisma.hpp"


Prisma::Prisma() :posicion(0,0,0,true), tam(0,0,0,true)
{}

// Prisma normal
Prisma::Prisma(const Vector3& _posicion, const Vector3& _tam, const bool _esAABB) :
	posicion(_posicion), tam(_tam), esAABB(_esAABB)//, caras[0]()
{}




// Devuelve el minimo de la coord i
float minPos(const std::vector<std::shared_ptr<Figura>>& prismas, const int i) {
	// std::cout <<"si\n";
	float min = prismas[0]->getBoundingBox()->getPos()[i];
	// std::cout <<"b\n";
	// std::cout <<"minpos\n";
	for (auto p : prismas) {
		// std::cout <<"forrrr\n";
		auto box = p->getBoundingBox();
		if (!box->esInfinito()){
			min = (box->getPos()[i] < min) ? box->getPos()[i] : min;
		}
	}

		// std::cout <<"fin minpos\n";
	return min;
}


// Devuelve el minimo de la coord i
float maxPos2(const std::vector<std::shared_ptr<Figura>>& prismas, const int i) {
	// std::cout <<"mmmmmmmmmmmm\n";
	float max = prismas[0]->getBoundingBox()->getPos()[i] + prismas[0]->getBoundingBox()->getTam()[i];
	for (auto p : prismas) {
		auto box = p->getBoundingBox();

		if (!box->esInfinito()){
			float pos2 = box->getPos()[i] + box->getTam()[i];
			max = (pos2 > max) ? pos2 : max;
		}
		// std::cout << i << " max: " << max << std::endl;

				// std::cout <<"fin maxpos\n";
	}
	return max;
}

// Constructor del prisma bounding box del vector de figuras
Prisma::Prisma(const std::vector<std::shared_ptr<Figura>>& bboxes)
: posicion(minPos(bboxes, 0), minPos(bboxes, 1), minPos(bboxes, 2), true), esAABB(true)
{
	tam.setCoords({maxPos2(bboxes, 0)-posicion[0], maxPos2(bboxes, 1)-posicion[1], maxPos2(bboxes, 2)-posicion[2]});
	// std::cout << "posicion......y tam......." << posicion<<std::endl << tam<< std::endl;
	// std::cout << posicion[0] << " " << posicion[1] << " "<< posicion[2] << "\n";
	// std::cout <<"mmmou3whwughuhmmmm\n" << tam << std::endl;
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

// TODO: COMPROBAR, COMPLETAMENTE A OJO
Vector3 normalDeCara(const int cara) {
	Vector3 normal(0, 0, 0, false);
	if (cara<3)	normal[cara] = -1;
	else {
		normal[cara-3] = 1;
	}
	return normal;
}


// TODO: comprobar!!!!
Vector3 Prisma::getNormal(const Vector3& pto) const {
	Vector3 p1 = getPos(); // esquina 1
	Vector3 p2 = p1 + getTam(); // esquina 2
	Vector3 p1_pto = abs(pto-p1); // Distancia al pto 1
	Vector3 p2_pto = abs(p2-pto); // Dist al 2
	double min = p1_pto[0]; // Buscamos la minima en las 6 posibilidades:
	int minIdx = 0; // indice del minimo
	for (int i = 1; i < 3; i++) {
		if (p1_pto[i] < min) {
			min = p1_pto[i];
			minIdx = i;
		}
	}
	for (int i = 0; i < 3; i++) {
		if (p2_pto[i] < min) {
			min = p2_pto[i];
			minIdx = i+3;
		}
	}
	return normalDeCara(minIdx);
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

// Devuelve true sii alguna coord i de a, es mayor que otra distinta j de b
// (EX i,j en 0,3, con i!=j, tq a[i] > b[j])
bool compAmayorCompB(const Vector3& a, const Vector3& b) {
	bool mayor = true;
	for (int i = 0; i<3; i++) { // coord del primero
		for (int j = 0; j<3; j++) { // y del segundo
			if (i!=j) { // Las compara si no son la misma
				mayor &= a[i] > b[j];
				if (mayor) {
					std::cout << a[i] << ">" << b[j] << "\ncon " << i << "," <<j<<std::endl;
					return true; // solo necesita encontrar una
				}
			}
		}
	}
	return mayor; // si llega aqui es falso
}


// TODO: Muy optimizable (importante si queremos poner muchas figuras con el BVH)
// True sii el rayo desde <origen>, hacia <dir> intersecta con el Prisma
// adaptado de: https://developer.arm.com/documentation/100140/0302/advanced-graphics-techniques/implementing-reflections-with-a-local-cubemap/ray-box-intersection-algorithm
std::optional<Figura::InterseccionData> Prisma::interseccion(const Vector3& origen, const Vector3& dir) const {

	// std::cout << "aja\n";
	// Vector3 a = getPos(); // primera esquina
	// Vector3 b = a + getTam(); // segunda
	// Vector3 tA = dividirComponentes((a - origen), dir); // Primera esquina
	// Vector3 tB = dividirComponentes((b - origen), dir); // Segunda
	//
	// // std::cout<<"dir: "<< dir << std::endl;
	// // std::cout<<"1/dir: "<< 1/dir << std::endl;
	// // std::cout<<"a: "<< a << std::endl;
	// // std::cout<<"tA: "<< tA << std::endl;
	// double t1 = tA.getMaxComponente();
	// double t2 = tB.getMinComponente();
	// if (t1>t2) {
	// 	std::swap(t1,t2);
	// 	std::swap(tA, tB);
	// }
	// std::cout << "tA\n" << tA << "\ntB\n" << tB<< std::endl;
	// std::cout << "t1: " << t1 << "\nt2: " << t2<< std::endl;
	// if (compAmayorCompB(tA, tB)) // True sii alguna comp de a es mayor que otra comp de b
	// 	return 0;
	// // (t0x > t1y || t0y > t1x)          (tmin > t1z || t0z > tmax)
	// // if (tA[0] > tB[1] || tA[1] > tB[0] || tA[2] > tB[2] || tA[2] > t2)
	//
	// return t1;
////////////////////////////////////////////////////////////// V2 ////////////////////////////////////////////777
// Adaptado de https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	Vector3 invdir=1/dir;
	Vector3 min =getPos();
	Vector3 max =min+getTam();
	float tmin=0;
	float tmax=0;
	float tymin=0;
	float tymax=0;
	float tzmin=0;
	float tzmax=0;
/*	float tmin = (min[0] - origen[0]) / dir[0];
  float tmax = (max[0] - origen[0]) / dir[0];

  if (tmin > tmax) std::swap(tmin, tmax);*/
	if (esAABB && contiene(origen)) { // si el origen está dentro del prisma, devolvemos el pto directamente
  	return InterseccionData{0, origen};
	}

  if (invdir[0] >= 0) {
    tmin = (min[0] - origen[0]) * invdir[0];
    tmax = (max[0] - origen[0]) * invdir[0];
	}
	else {
	    tmin = (max[0] - origen[0]) * invdir[0];
	    tmax = (min[0] - origen[0]) * invdir[0];
	}
	  if (invdir[1] >= 0) {
	    tymin = (min[1] - origen[1]) * invdir[1];
	    tymax = (max[1] - origen[1]) * invdir[1];
	}
	else {
	    tymin = (max[1] - origen[1]) * invdir[1];
	    tymax = (min[1] - origen[1]) * invdir[1];
	}
	  if (invdir[2] >= 0) {
	    tzmin = (min[2] - origen[2]) * invdir[2];
	    tzmax = (max[2] - origen[2]) * invdir[2];
	}
	else {
	    tzmin = (max[2] - origen[2]) * invdir[2];
	    tzmax = (min[2] - origen[2]) * invdir[2];
	}
  //float tymin = (min[1] - origen[1]) / dir[1];
  //float tymax = (max[1] - origen[1]) / dir[1];

  //if (tymin > tymax) std::swap(tymin, tymax);

  if ((tmin > tymax) || (tymin > tmax))
      return std::nullopt;

  if (tymin > tmin)
      tmin = tymin;

  if (tymax < tmax)
      tmax = tymax;

  //float tzmin = (min[2] - origen[2]) / dir[2];
  //float tzmax = (max[2] - origen[2]) / dir[2];

  //if (tzmin > tzmax) std::swap(tzmin, tzmax);

  if ((tmin > tzmax) || (tzmin > tmax))
      return std::nullopt;

  if (tzmin > tmin)
      tmin = tzmin;

  if (tzmax < tmax)
      tmax = tzmax;
		// std::cout << "aja\n";
	if (tmin<0) return std::nullopt;
  return InterseccionData{tmin, origen+tmin*dir};
}


// Devuelve la AABB (prisma alineado con los ejes) que envuelve a la figura
std::shared_ptr<Prisma> Prisma::getBoundingBox() const {
	// TODO: actualizar cuando se implementen prismas rotados
	Prisma box = *this;
	box.esAABB = true;
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

// Devuelve un pto aleatorio dentro del prisma
Vector3 Prisma::getPtoAleatorio() const {
	GeneradorAleatorio gen;
	double random01 = gen.rand01();
	Vector3 pto = posicion + random01*tam;
	return pto;
}

// Devuelve un pto aleatorio dentro del prisma
Vector3 Prisma::getPtoAleatorio(const GeneradorAleatorio& gen) const {
	double random01 = gen.rand01();
	return posicion + random01*tam;
}


// Devuelve el prisma minimo que envuelve a p1 y p2
// Su primera esquina sera el minimo de cada coordenada de las primeras esquinas
// La segunda esquina, el maximo de cada coord de las segundas esquinas
std::shared_ptr<Prisma> combinar(const std::shared_ptr<Prisma> p1, const std::shared_ptr<Prisma> p2) {
	if (p1->esInfinito() || p2->esInfinito()) return std::make_shared<CajaInfinita>(CajaInfinita());
	//else if (p2->esInfinito()) return p1;
	// Si ninguno es infinito, los combinamos
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
	return std::make_shared<Prisma>(Prisma(posCombinacion, pos2Combinacion-posCombinacion, true));
}

std::pair<std::shared_ptr<Prisma>, std::shared_ptr<Prisma>> Prisma::partirEnEje(const int eje) const {
	Vector3 nuevoTam = tam;
	nuevoTam[eje] -= tam[eje]/2.0; // Tamaño de las dos, el mismo con un eje a la mitad
	// std::cout << tam << " .... " << nuevoTam << std::endl << tam[eje]/2.0;
	Prisma primero(posicion, nuevoTam); // Empieza en la misma esquina
	primero.esAABB = true;
	Vector3 pos2 = posicion;
	pos2[eje] += tam[eje]/2.0; // desplazado la mitad de su tam en el eje dado
	Prisma segundo(pos2, nuevoTam);
	segundo.esAABB = true;
	return std::pair<std::shared_ptr<Prisma>,std::shared_ptr<Prisma>>(std::make_shared<Prisma>(primero), std::make_shared<Prisma>(segundo));
} // return primero, segundo   <----- esto es lo de arriba en python :/

/********************* Rotables **************************/

PrismaRotable::PrismaRotable(const Vector3& _posicion, const Vector3& _tam) :
Prisma(Vector3(0), _tam), base(FRONT,LEFT,UP,posicion)
{
	baseInversa = base.inversa(); // mas eficiente, supongo
}


void PrismaRotable::rotar(const Matriz4& rotacion) {
	base = rotacion * base;
	baseInversa = base.inversa();
}



Vector3 PrismaRotable::getPos() const {
	return base[3];
}


// Devuelve la normal de la figura en el <pto>
Vector3 PrismaRotable::getNormal(const Vector3& pto) const {
	return Prisma::getNormal(base * pto);
}
//
// bool PrismaRotable::contiene(const Vector3& p) const {
// 	return
// }

std::optional<Figura::InterseccionData> PrismaRotable::interseccion(const Vector3& origen, const Vector3& dir) const {
	auto interseccion = Prisma::interseccion(base * origen, base * dir);
	if (interseccion) {
		interseccion->punto = baseInversa * interseccion->punto;
	}
	return interseccion;
}





/******** CAJAS INFINITAS **********/

// Siempre intersecta
std::optional<Figura::InterseccionData> CajaInfinita::interseccion(const Vector3& origen, const Vector3& dir) const {
	return InterseccionData{1, Vector3()};
}

// Introspeccion a lo bestia:
bool Prisma::esInfinito() const  {
	return false;
}

bool CajaInfinita::esInfinito() const  {
	// std::cout << "AAAAAAAAAAAAAJJJJJJJJJJJJAAAAAAAAAAAAa\n";
	return true;
}
