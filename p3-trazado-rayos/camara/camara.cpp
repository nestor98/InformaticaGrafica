#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
//#include "../p1-geometria/Vector.hpp"

#include "camara.hpp"

#include "Imagen.hpp"

std::random_device rd;  //Will be used to obtain a seed for the random number engine

Camara::Camara(const Vector3& _posicion, const Vector3& _front, const Vector3& _left, const Vector3& _up,
							 const int _pixelesX, const int _pixelesY, const int _rayosPixel) :
	posicion(_posicion), front(_front), left(_left), up(_up), pixelesX(_pixelesX), pixelesY(_pixelesY),
	rayosPixel(_rayosPixel), gen(rd()), dis(0.0,1.0), base(left, up, front, posicion)
{
	std::cout << "pixelesX: " << pixelesX << std::endl;
	std::cout << "pixelesY: " << pixelesY << std::endl;
	baseInversa = base.inversa();
	std::cout<< "Matriz de cambio de base:\n" << base << std::endl<< baseInversa<< std::endl;
	// // numeros aleatorios entre 0 y 1 (de https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution)

  // gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	// dis(0.0, 1.0);
}

double Camara::rand01() const {
	return dis(gen);
}

std::string Camara::to_string() const {
	return "posicion: " + posicion.to_string() + "\ndireccion: " + front.to_string();
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

// devuelve el vector del iesimo pixel de la camara
Vector3 Camara::getRayoPixel(const int i) const{
	int x = i%(pixelesX); // coord x
	int y = i/(pixelesX);

	double xLeft = x+rand01();
	xLeft = -(xLeft/(pixelesX/2.0)-1.0);
	double yUp = y+rand01();
	yUp = -(2.0*yUp/double(pixelesY)-1.0);
	// std::cout << xLeft << "\t" << yUp << std::endl;
	Vector3 dirCamara(xLeft, yUp, 1, false);
	//std::cout << "Dircamara: " << dirCamara << std::endl;
	return base*dirCamara; // se devuelve en coords del mundo
}

int Camara::getRayosPorPixel() const {
	return rayosPixel;
}




// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Camara& c) {
	os << c.to_string();
	return os;
}
