#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>


#include "Imagen.hpp"
#include "Vector3.hpp"

class Textura{
    protected:
        Imagen tex; //Para conseguir una imagen en ppm p3 convertir online a ppm que lo da en p6 y usar comando: convert foo.ppm -compress none bar.ppm
        int ancho;
        int largo;
        Vector3 pos;
    public:
    Textura();
    Textura(Imagen& i, int _ancho, int _largo, Vector3 _pos);
    std::array<double, 3> getEmision(const Vector3& dir)const;
};
