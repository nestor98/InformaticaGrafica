#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include "textura.hpp"


Textura::Textura(Imagen& i, int _ancho, int _largo, Vector3 _pos){
     tex=i;
     ancho=_ancho;
    largo=_largo;
    pos=_pos;
}

Textura::Textura(): ancho(0), largo(0)
{}
std::array<double, 3> Textura::getEmision(Vector3 dir) const{
    int x=dir[0];
    int y=dir[1];
    if(x<pos[0]){
        x=pos[0]-x;
    }else{
        x=x-pos[0];
    }
    if(y<pos[1]){
        y=pos[1]-y;
    }else{
        y=y-pos[1];
    }
    return tex.getPixel(x, y);

}
