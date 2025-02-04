#include <iostream>
#include <string>

#include "figura.hpp"
#include "prisma.hpp"

Figura::Figura(const std::array<double, 3> _e) :
e(_e), emite(true), textura(false), conBumpMap(false)
{}

Figura::Figura(const Material& _m)
: m(_m), emite(false), textura(false), conBumpMap(false)
{}


// TODO: revisar emite, gestionar texturas+materiales etc
Figura::Figura(std::shared_ptr<Textura> _tex)
: emite(true), tex(_tex), textura(true), conBumpMap(false)
{
	// std::cout << "ah pos si\n";
}

Figura::Figura() : Figura(std::array<double, 3>({0,0,0}))
{
	setRandomColor();
	// setColorFromPos();
}

void Figura::setMaterial(const Material& _m) {
	emite = false;
	m = _m;
}

void Figura::setTextura(std::shared_ptr<Textura> _tex){
	textura=true;
	tex=_tex;
}

void Figura::setBumpMap(std::shared_ptr<Textura> _bm){
	conBumpMap=true;
	bumpMap=_bm;
	// std::cout << "Tengo bump map y textura " << textura  << '\n';
}

Material Figura::getMaterial() const {
	return m;
}

std::string Figura::to_string() const {
	return "Figura no tiene to_string";
}

// Devuelve la normal de la figura en el <pto>
Vector3 Figura::getNormal(const Vector3& pto) const {
	return Vector3();
}

Color Figura::getEmision(const Vector3& dir) const {
	if(!textura){
		return e;
	} else {
		auto textura= tex->getEmision(dir);
		return textura;
	}
}

	bool Figura::tieneTextura() const{
		return textura;
	}

// Vector3 Figura::getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen) const {
// 	if (!emite) return m.getVectorSalida(base, gen);
// 	else {
// 		std::cerr << "No tengo material" << '\n';
// 		exit(1);
// 	}
// }


Matriz4 Figura::getBase(const Vector3& pto) {
	try {
		if (!conBumpMap){
			return getBase(this->getNormal(pto), pto);
		}
		else {
			Matriz4 base;
			bumpMap->bump(pto, *this, base);
			// Matriz4 base2=getBase(this->getNormal(pto), pto);
			// if (base[2]!=base2[2]) {
			// 	std::cerr << "Normales distintas???......." << '\n';
			// 	exit(1);
			// }

			// std::cout << "pto: " << pto << "\nBase de bump:" << '\n' << base;
			return base;
		}
	}
	catch (std::string e) {
		std::cerr << "error en getBase " << e << '\n';
	}
}

Matriz4 Figura::getBase(const Vector3& normal, const Vector3& pto) {
	Vector3 normalVec = normal; normalVec.setVector();
	Vector3 ptoPunto = pto; ptoPunto.setPunto();
	Matriz4 base = baseFromVector(normalVec, ptoPunto);
	return base;
}


std::shared_ptr<Prisma> Figura::getBoundingBox() const {
	std::cout << "CUIDAOOOOOOOOOOOOOOOOOOOOOOOOOOOO------------------------------------------------------\n";
	return bbox;
}

void Figura::setColor(const Color& _e) {
	e = _e;
}



void Figura::setColor(const double r, const double g, const double b) {
	e.setRGB(r,g,b);
}

void Figura::setColor(const std::array<double, 3> _e = {1,0,0}) {
	e.setRGB(_e);
}

bool Figura::esEmisor() const {
	return emite;
}



// color aleatorio
void Figura::setRandomColor() {
	//std::cout << utils.rand01()<<std::endl;
	e.setRandom();
	// e = {0,0,0};// TODO{utils.rand01(),utils.rand01(),utils.rand01()};
}

// True sii el rayo desde <origen>, hacia <dir> intersecta con la esfera
std::optional<Figura::InterseccionData> Figura::interseccion(const Vector3& origen, const Vector3& dir) const {
	return std::nullopt;
}
// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Figura& c) {
	os << c.to_string();
	return os;
}
// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Figura> c) {
	os << c->to_string();
	return os;
}

// TODO: de momento devuelve el centroide de su bounding box, que no tiene
// pq ser el mismo que el suyo propio, ya veremos si hay que cambiarlo
Vector3 Figura::getCentroide() const {
	return getBoundingBox()->getCentroide();
}


// Le cambia el color en funcion de la posicion
void Figura::setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max) {
	e.setFromPosGrad(pto, min, max);
}
