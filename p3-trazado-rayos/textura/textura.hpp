#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>


#include "Imagen.hpp"
#include "color.hpp"
#include "Vector3.hpp"
class Figura;

class Textura{
    protected:
        Imagen tex; //Para conseguir una imagen en ppm p3 convertir online a ppm que lo da en p6 y usar comando: convert foo.ppm -compress none bar.ppm
        int ancho;
        int alto;
        Vector3 pos;
        float multColor;
        Matriz4 base, baseInversa; // Bases para local->mundo, mundo->local
        bool _esBumpMap;
        float maxDespl;
    public:
    Textura();
    Textura(const Imagen& i, const float _ancho, const float _alto,
      const Vector3& _pos, const float& _multColor=1);
    Color getEmision(const Vector3& pto)const;

    // Rota la textura segun la matriz de rotacion, alrededor de pos (esquina sup izquierda)
    void rotar(const Matriz4& rotacion);

    //************************************************************************
    // BUMP MAPPING:
    bool esBumpMap() const;

    Vector3 desplazar(const Vector3& pto, const Figura& fig) const;

    void bump(const Vector3& pto, const Figura& fig, Matriz4& baseBump) const;

    void setMaxDesplaz(const float& despl);
    float getMaxDesplaz() const;

    // Cambia su pos (esquina sup izq)
    void setPos(const Vector3& nuevaPos);


};
