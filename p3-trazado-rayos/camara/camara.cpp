#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
//#include "../p1-geometria/Vector.hpp"

#include "camara.hpp"

#include "Imagen.hpp"

Camara::Camara(const Vector3& _posicion, const Vector3& _front, const Vector3& _left, const Vector3& _up,
							 const int _pixelesX, const int _pixelesY, const int _rayosPixel) :
	posicion(_posicion), front(_front), left(_left), up(_up), pixelesX(_pixelesX), pixelesY(_pixelesY),
	rayosPixel(_rayosPixel), base(left, up, front, posicion)
{
	// std::cout << "pixelesX: " << pixelesX << std::endl;
	// std::cout << "pixelesY: " << pixelesY << std::endl;
	// // baseInversa = base.inversa();
	// std::cout<< "Matriz de cambio de base:\n" << base << std::endl<< baseInversa<< std::endl;

}


Camara::Camara(const Vector3& _posicion, const Vector3& objetivo, const Vector3& _up, const double fov, const int _pixelesX,
	const int _pixelesY, const int _rayosPixel) :
		posicion(_posicion), up(_up),
		pixelesX(_pixelesX), pixelesY(_pixelesY),rayosPixel(_rayosPixel)
		{
			left = cross(up, normalizar(objetivo)) * (double(pixelesX)/double(pixelesY));
			std::cout << "up x objetivo = " << cross(up, objetivo) << '\n';
			std::cout << "left: " << left << "\nLEFT:" << LEFT << '\n';
			std::cout << "up: " << up << "\nUP:" << UP << '\n';
			front = cross(left, up);
			std::cout << "front: " << front << "\nFRONT:" << FRONT << '\n';
			setFOV(fov);

		}



std::string Camara::to_string() const {
	return "posicion: " + posicion.to_string() + "\nbase: " + base.to_string();
}

Vector3 Camara::getPos() const {
	return posicion;
}
Vector3 Camara::getFront() const {
	return front;
}
Vector3 Camara::getLeft() const {
	return left;
}
Vector3 Camara::getUp() const {
	return up;
}

int Camara::getPixelesX() const {
	return pixelesX;
}

int Camara::getPixelesY() const {
	return pixelesY;
}
int Camara::getNumPixeles() const {
	return pixelesX*pixelesY;
}

// devuelve el vector del iesimo pixel de la camara
Vector3 Camara::getRayoPixel(const int i) const{
	int x = i%(pixelesX); // coord x
	int y = i/(pixelesX);

	double xLeft = x+gen.rand01();
	xLeft = -(xLeft/(pixelesX/2.0)-1.0);
	double yUp = y+gen.rand01();
	yUp = -(2.0*yUp/double(pixelesY)-1.0);
	// std::cout << xLeft << "\t" << yUp << std::endl;
	Vector3 dirCamara(xLeft, yUp, 1, false);
	//std::cout << "Dircamara: " << dirCamara << std::endl;
	return normalizar(base*dirCamara); // se devuelve en coords del mundo
}

// devuelve el vector del iesimo pixel de la camara
Vector3 Camara::getRayoCentroPixel(const int i) const{
	int x = i%(pixelesX); // coord x
	int y = i/(pixelesX);

	double xLeft = x+0.5;
	xLeft = -(xLeft/(pixelesX/2.0)-1.0);
	double yUp = y+0.5;
	yUp = -(2.0*yUp/double(pixelesY)-1.0);
	// std::cout << xLeft << "\t" << yUp << std::endl;
	Vector3 dirCamara(xLeft, yUp, 1, false);
	//std::cout << "Dircamara: " << dirCamara << std::endl;
	return normalizar(base*dirCamara); // se devuelve en coords del mundo
}






int Camara::getRayosPorPixel() const {
	return rayosPixel;
}

// Cambia la distancia focal de la camara
void setFocalLength(const double mm) {

}

// Cambia la distancia focal de la camara
void Camara::setFOV(const double fov) {
	// sacado con un dibujo y tan(angulo) = opuesto/adyacente
	// con angulo=fov, opuesto=up, adyacente=front
	float modFront = left.getModulo()/tan(fov/2.0);
	 std::cout << "modFront: " << modFront << std::endl;
	front.setModulo(std::abs(modFront));
		 std::cout << "modFrontdespues: " << modFront << std::endl;

	base.setCambioBase(left, up, front, posicion);
}

// Dada una anchura de una escena a renderizar, devuelve a que distancia tiene que estar
// la camara actual del centro
void Camara::setOrigenParaTarget(const Vector3& target)  {
	posicion = target-front;
}

void Camara::rotarAlrededorDeTarget(const Matriz4& rotacion) {
	Vector3 target = posicion + front;
	rotacion[3] = target;
	// TODO: algo asi????
	posicion = rotacion*posicion;
	front = rotacion*front;
	up = rotacion*up;
	left = rotacion*left;
}



// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Camara& c) {
	os << c.to_string();
	return os;
}
