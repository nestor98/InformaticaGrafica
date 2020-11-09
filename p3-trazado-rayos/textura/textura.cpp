#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include "textura.hpp"


Textura::Textura(Imagen& i, float _ancho, float _largo, Vector3 _pos)
  : tex(i), ancho(_ancho), largo(_largo), pos(_pos-Vector3(_ancho/2.0, 0, -_largo/2.0,false))
{
  // std::cout << "_pos << pos" << std::endl;
  // std::cout << _pos << std::endl << pos << std::endl;
    //  tex=i;
    //  ancho=_ancho;
    // largo=_largo;
    // pos=_pos;
    std::cout << "imagen tiene "<< tex.to_string();
    // tex.guardar("out/vamohave.ppm");
    for (int i = 0; i<100; i++) {
      auto color = tex.getPixel(i) ;
      std::cout << i <<" ..... "<< color[0] << " " << color[1] <<" "<<color[2] << std::endl;
    }
    std::cout<<"yeeyeyeyey\n";
}

Textura::Textura(): ancho(0), largo(0)
{
  std::cout<<"vacio\n";
}

std::array<double, 3> Textura::getEmision(const Vector3& pto) const{
  // std::cout <<"por si acaso\n";
    double ptox=pto[0]; // [0, tex.getPixelesX)
    double x = pto[0];
    double y=pto[2]; // [0, tex.getPixelesY)
    x = pto[0]-pos[0];
    y = -(y-pos[2]);
    x = x/ancho;
    y = y/largo;

    // std::cout << int(y*tex.getNumPixelsY()) << "  " << int(x*tex.getNumPixelsX()) << std::endl;
    return tex.getPixel(int(y*(tex.getNumPixelsY()-1)), int(x*(tex.getNumPixelsX()-1)));

    // if(x<pos[0]){
    //     x=pos[0]-x;
    // }else{
    //     x=x-pos[0];
    // }
    // if(y<pos[2]){
    //     y=pos[2]-y;
    // }else{
    //     y=y-pos[2];
    // }

    // // std::cout << int(x*tex.getNumPixelsX()) << "," << int(y*tex.getNumPixelsY()) << std::endl;
    // double x = pto[0]-pos[0];
    // int xInt = x;
    // x -= double(xInt);
    // double y = pto[2]-pos[2];
    // int yInt = y;
    // y -= double(yInt);
    //
    // int col = xInt * tex.getNumPixelsX() / ancho;
    // col += x * tex.getNumPixelsX() / ancho;
    // int fila = yInt * tex.getNumPixelsY() / largo;
    // // fila += y * tex.getNumPixelsY() / largo;
    //
    // return tex.getPixel(fila, col);
    // return tex.getPixel(int(y*tex.getNumPixelsY()), int(x*tex.getNumPixelsX()));

}
