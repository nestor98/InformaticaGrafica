#include <iostream>
#include <string>


#include "esfera.hpp"

Esfera::Esfera(const Vector3& _posicion, const float _radio) :
	 posicion(_posicion), radio(_radio)
{
	setBoundingBox();
}
Esfera::Esfera(const Vector3& _posicion, const float _radio,std::shared_ptr<Textura> _tex) :
	Figura(_tex), posicion(_posicion), radio(_radio)
{
	std::cout << "hola\n";
	setBoundingBox();
}
//
// Esfera::Esfera(const Vector3& _posicion, const float _radio, const std::string fichero_textura) :
// 	Figura(fichero_textura, _radio*2.0, _radio*2.0), posicion(_posicion), radio(_radio)
// {
// 	std::cout << "hola\n";
// 	setBoundingBox();
// }

std::string Esfera::to_string() const {
	return "--- Esfera:\nposicion: " + posicion.to_string() + "\nradio: " + std::to_string(radio);
}

Vector3 Esfera::getPos() const {
	return posicion;
}

float Esfera::getRadio() const {
	return radio;
}


// True sii el rayo desde <origen>, hacia <dir> intersecta con la esfera
double Esfera::interseccion(const Vector3& origen, const Vector3& dir) const {
	// sustituyendo en el sistema con:
		// p = o+td
		// |p|^2-r^2=0
	// Tenemos:
	// d^2*t^2+2o*d*t+o^2−r^2
	// Para resolver la ecuacion:
	double a = dir*dir;//;*dir; // d^2
	// TODO: DEBUG, siempre da delta=0 (creo que es culpa de dir que siempre es lo mismo)
	// std::cout<<"a: "<<a<<std::endl;
	// double b = 2.0*(origen*dir); // 2od
	Vector3 o_centro = origen-getPos(); // o-c
	double b = 2.0*dir*o_centro; // 2d(o-c)
	// std::cout<<"b: "<<b<<std::endl;
	double r = getRadio();
	// std::cout<<"r: "<<r<<std::endl;
	// std::cout<<"origen*origen: "<<origen.getModulo()*origen.getModulo()<<std::endl;
	double c = o_centro*o_centro-r*r; // (o-c)^2-r^2
	// std::cout<<"c: "<<c<<std::endl;

	double delta = b*b-4.0*a*c;

	// std::cout<<"delta: "<<delta<<std::endl;
	// si delta>0, dos soluciones (dos puntos de interseccion),
	if (delta>0) {
		double sqrtDelta = sqrt(delta);
		if (b<sqrtDelta) {
			return (-b+sqrtDelta)/(2*a);
		}
		else {
			return (-b-sqrtDelta)/(2*a);
		}
	}
	// si delta==0, una solucion (pto tangente)
	else if (delta==0) {
		return -b/2*a;
	}
	// si delta<0, no hay solucion (no intersecta)
	else {
		return 0.0;
	}
	return delta; // intersecta
}


Vector3 Esfera::getNormal(const Vector3& pto) const {
	return pto-posicion; 
}


/*void Esfera::addTextura(Textura _tex){ //NO FUNCIONA
	tex=_tex;
	std::cout<<"que pasa"<<std::endl;
	textura=true;
}*/

void Esfera::setBoundingBox() {
	// la caja va del centro-{radio,radio,radio} hasta el centro+{radio,radio,radio}
	Vector3 despl(radio); // 3 componentes radio, radio, radio
	bbox = std::shared_ptr<Prisma>(new Prisma(posicion - despl, despl*2.0));
}

std::shared_ptr<Prisma> Esfera::getBoundingBox() const {
	return bbox;
}
