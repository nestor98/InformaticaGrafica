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
/*double interseccion(const Vector3& origen, const Vector3& dir) const {
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
	//std::cout<<"r: "<<r<<std::endl;
	// std::cout<<"origen*origen: "<<origen.getModulo()*origen.getModulo()<<std::endl;
	double c = o_centro*o_centro-r*r; // (o)^2-r^2
	//std::cout<<"c: "<<c<<std::endl;

	double delta = b*b-4.0*a*c;
	//std::cout << "INTERSECTANDO ESF\n";

	// si delta>0, dos soluciones (dos puntos de interseccion),
	if (delta>0) {
		double sqrtDelta = sqrt(delta);
		//std::cout<<"raiz:"<<sqrtDelta<<std::endl;
		double t = (-b+sqrtDelta)/(2*a);
		//std::cout<<"t: "<<t<<std::endl;
		if (t<=0) return (-b-sqrtDelta)/(2*a);
		else return t;
		// if (b<sqrtDelta) {
		// 	return (-b+sqrtDelta)/(2*a);
		// }
		// else {
		// 	return (-b-sqrtDelta)/(2*a);
		// }




	}
	// si delta==0, una solucion (pto tangente)
	else if (delta==0) {
		return -b/(2*a);
	}
	// si delta<0, no hay solucion (no intersecta)
	else {
		return 0.0;
	}
	return delta; // intersecta
}*/


std::optional<Figura::InterseccionData> Esfera::interseccion(const Vector3& origen, const Vector3& dir) const
{
   // printf (">>>%f,%f,%f<< \n",  ray.direccion.x,  ray.direccion.y,  ray.direccion.z);
	double distancia=0;
  double a = dir*dir;
  Vector3 ro_sc = origen-getPos(); // Normalizar este vector para efectos psicodelicos
  double b = 2.0 *dir*ro_sc;
	double mod_ro_sc_sq = ro_sc.getModuloSq();
  double y = mod_ro_sc_sq - (getRadio()*getRadio());
	// NI IDEA de como funcionaba asi:
	// double y = ro_sc*ro_sc - (getRadio()*getRadio());

	//std::cout << "radio " << getRadio() << '\n';


  // double discriminante = b*b - 4.0 * a * y;
	  double discriminante = b*b - 4.0* a * y;
	// std::cout << "discriminante " << discriminante << '\n';

  if (discriminante >=0 ){
  	double RaizDiscriminante = sqrt(b*b - 4.0 * a * y);
    double t1 = (-b-RaizDiscriminante)/(2.0*a);
    double t2 = (-b+RaizDiscriminante)/(2.0*a);
    if (t1<t2 && t1>0){
      distancia =  t1;
    }
    else{
      distancia =  t2;
    }
		// std::cout << "yep "<< distancia << "\n";
		if (distancia<=0) return std::nullopt; // intersecta detras del origen, como si no intersectara

		return Figura::InterseccionData{distancia, origen+distancia*dir};
  }
  else {
		//std::cout << "nope" << '\n';
    return std::nullopt; // no intersecta
  }

}

Vector3 Esfera::getNormal(const Vector3& pto) const {
	Vector3 normal = pto-posicion;
	//Vector3 normalizada2 = normal / normal.getModulo();

	//normal.normalizar();
	// if (normalizada2 != normal) {
	// 	std::cout << normalizada2 << "    "<<normal<<std::endl;
	// }
	// std::cout <<"Pto: " << pto <<"\npos" <<posicion<<"\nnormal normalizada: " << normal << std::endl;
	return normalizar(normal);
}


/*void Esfera::addTextura(Textura _tex){ //NO FUNCIONA
	tex=_tex;
	std::cout<<"que pasa"<<std::endl;
	textura=true;
}*/

void Esfera::setBoundingBox() {
	// la caja va del centro-{radio,radio,radio} hasta el centro+{radio,radio,radio}
	Vector3 despl(radio); // 3 componentes radio, radio, radio
	bbox = std::shared_ptr<Prisma>(new Prisma(posicion - despl, despl*2.0, true));
}

std::shared_ptr<Prisma> Esfera::getBoundingBox() const {
	return bbox;
}
