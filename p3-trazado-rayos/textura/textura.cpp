#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include "textura.hpp"
#include "figura.hpp"


Textura::Textura(const Imagen& i, const float _ancho, const float _alto, const Vector3& _pos)
  : tex(i), ancho(_ancho), alto(_alto), pos(_pos), _esBumpMap(false)
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

Textura::Textura(): ancho(0), alto(0), _esBumpMap(false)
{
  std::cout<<"vacio\n";
}


void Textura::setMaxDesplaz(const float& despl) {
  maxDespl = despl;
  _esBumpMap=true;
}

bool Textura::esBumpMap() const {
  return _esBumpMap;
}

float Textura::getMaxDesplaz() const {
  if (_esBumpMap) return maxDespl;
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
    double y=ptoLocal[1]; // [0, tex.getPixelesY)
    y = -(y);
    x = x/ancho;
    y = y/alto;

    // std::cout << int(y*tex.getNumPixelsY()) << "  " << int(x*tex.getNumPixelsX()) << std::endl;
    return Color(tex.getPixel(int((y)*(tex.getNumPixelsY()-1)), int((1.0-x)*(tex.getNumPixelsX()-1))));
}

Vector3 Textura::desplazar(const Vector3& pto, const Figura& fig) const {
  float desplazamiento = getEmision(pto).getPromedio() * getMaxDesplaz();
  // std::cout << "despl: " << desplazamiento << '\n';
  return alejarDeNormal(pto, fig.getNormal(pto), desplazamiento);
}

void Textura::bump(const Vector3& pto, const Figura& fig, Matriz4& baseBump) const{
    float du = 1e-2; // tamaño del paso para diferenciacion
    Vector3 ptoDesplazado = desplazar(pto, fig);
    // if (pto - ptoDesplazado) {
    //   std::cerr << "pto es ptoDesplazado!!!" << '\n';
    // }
    Vector3 Pu = pto + base[0]*du; // pto desplazado hacia u (base[0])
    Pu = desplazar(Pu, fig); // Desplazado otra vez segun el bump map
    Vector3 Pv = pto + base[1]*du; // pto desplazado hacia v (base[1])
    Pv = desplazar(Pv, fig); // Desplazado otra vez segun el bump map
    if (Pu==pto|| Pv==pto) {
      std::cerr << "oigasnoagdsnonasdn" << '\n'; exit(1);
    }
    // Para calcular la normal, las dos tangentes en ptoDesplazado:
    Vector3 Vu = normalizar(Pu-ptoDesplazado);
    Vector3 Vv = normalizar(Pv-ptoDesplazado);
    if (Vu == Vv) {
      std::cerr << "Vu y Vv son iguales!!!!\t" << Vu << '\n';
      exit(1);
    }
    if (!Vu.esVector() || !Vv.esVector() || ptoDesplazado.esVector()) {
      std::cerr << "Vu y Vv tienen que ser vectores, ptoDesplazado un punto!!!" << '\n';
      exit(1);
    }
    Vector3 n = normalizar(cross(Vu,Vv));
    // std::cout << "Vu: " << Vu << "\nVv: " << Vv << "\ncross: " << n << '\n';

    if (n*fig.getNormal(pto)<=0){// || n*fig.getNormal(pto)!=1) {
      std::cerr << "Vaadgangansdgi\n"; exit(1);
    }
    baseBump.setCambioBase(Vu, Vv, n, ptoDesplazado);
}
