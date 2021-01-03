#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include "textura.hpp"


Textura::Textura(const Imagen& i, const float _ancho, const float _alto, const Vector3& _pos)
  : tex(i), ancho(_ancho), alto(_alto), pos(_pos+LEFT*_ancho/2.0-UP*_ancho/2.0), esBumpMap(false)
{
  base.setCambioBase(UP, -LEFT, FRONT, pos);
  baseInversa = base.inversa();
  // std::cout << "_pos << pos" << std::endl;
   //std::cout << _pos << std::endl << pos << std::endl;
   //std::cout<<ancho<<"  ,   "<<alto<<std::endl;
    //  tex=i;
    //  ancho=_ancho;
    // alto=_alto;
    // pos=_pos;
    // std::cout << "imagen tiene "<< tex.to_string();
    // // tex.guardar("out/vamohave.ppm");
    // for (int i = 0; i<100; i++) {
    //   auto color = tex.getPixel(i) ;
    // }
}

Textura::Textura(): ancho(0), alto(0), esBumpMap(false)
{
  std::cout<<"vacio\n";
}


void Textura::setMaxDesplaz(const float& despl) {
  maxDespl = despl;
  esBumpMap=true;
}

float Textura::getMaxDesplaz() const {
  if (esBumpMap) return maxDespl;
  else return 0;
}

void Textura::rotar(const Matriz4& rotacion) {
	// Vector3 centroideMundo = base * tam/2; // 0+tam/2
	// centroideMundo[3]=1;
	// base[3] = centroideMundo;
	Vector3 esquina = base[3];
	// baseCentroide[3] = base[3]+baseInversa*tam/2;	baseCentroide[3][3] = 1;
	base =  rotacion * base;
  base[3] = esquina;
	//base[3] = //centroideMundo - tam/2; // para rotar alrededor del centroide
	baseInversa = base.inversa();
}


void Textura::setPos(const Vector3& nuevaPos) {
  base[3] = nuevaPos;
  baseInversa = base.inversa();
}


Color Textura::getEmision(const Vector3& pto) const{
  // std::cout <<"por si acaso\n";
    // double ptox=pto[0]; // [0, tex.getPixelesX)
    Vector3 ptoLocal = baseInversa * pto;
    double x = ptoLocal[0];//-pos[0];
    double y=ptoLocal[2]; // [0, tex.getPixelesY)
    y = -(y-pos[2]);
    x = x/ancho;
    y = y/alto;

    // std::cout << int(y*tex.getNumPixelsY()) << "  " << int(x*tex.getNumPixelsX()) << std::endl;
    return Color(tex.getPixel(int((y)*(tex.getNumPixelsY()-1)), int((1.0-x)*(tex.getNumPixelsX()-1))));
}
