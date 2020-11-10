#include <cmath> // seno

#include "color.hpp"
#include "utils.hpp"

Color::Color() : Color(0,0,0)
{}

Color::Color(const double _r, const double _g, const double _b)
: r(_r), g(_g), b(_b)
{}

Color::Color(const std::array<double,3> rgb) : Color(rgb[0], rgb[1], rgb[2])
{}

// Da un color para un pto, negro en min, blanco en max
// r primera coordenada, g segunda, b tercera
Color::Color(const Vector3& pto, const Vector3& min, const Vector3& max)
{
  setFromPosGrad(pto, min, max);
}

void Color::setFromPosGrad(const Vector3& pto, const Vector3& min, const Vector3& max)
{
  Vector3 pos = 1-dividirComponentes((max-pto),max-min); // Si se quita el 1- sale guay tb
	setRGB(pos[0], pos[1], pos[2]);
}

void Color::setFromPosExp(const Vector3& pto, const Vector3& min, const Vector3& max)
{
  Vector3 pos = 4.0*dividirComponentes((max-pto),max-min); // Si se quita el 1- sale guay tb
	setRGB((1+sin(pos[0])/2.0), (1+sin(pos[1]))/2.0, (1+sin(pos[2]))/2.0);
}

void Color::setRandom() {
  setRGB(utils.rand01(), utils.rand01(), utils.rand01());
}


std::array<double,3> Color::toArray() const {
  return std::array<double,3>{r,g,b};
}

double Color::getR() const {
  return r;
}
double Color::getG() const {
  return g;
}
double Color::getB() const {
  return b;
}

void Color::setR(const double _r) {
  r = _r;
}
void Color::setG(const double _g) {
  g = _g;
}
void Color::setB(const double _b) {
  b = _b;
}

void Color::setRGB(const double _r, const double _g, const double _b) {
  r = _r;
  g = _g;
  b = _b;
}

void Color::setRGB(const std::array<double,3> rgb) {
  setRGB(rgb[0], rgb[1], rgb[2]);
}


// componente (set, v[2] = 3)
double& Color::operator [](int i) {
  switch (i) {
    case 0: return r;
    break;
    case 1: return g;
    break;
    default: return b;
  }
}



// componente (get, a = v[2])
double Color::operator [](int i) const {
  switch (i) {
    case 0: return r;
    break;
    case 1: return g;
    break;
    default: return b;
  }
}



// +
Color operator + (const Color& c1, const Color& c2) {
  return Color(c1.getR()+c2.getR(), c1.getG() + c2.getG(), c1.getB() + c2.getB());
}

// -
Color operator - (const Color& c1, const Color& c2) {
  return Color(c1.getR()-c2.getR(), c1.getG() - c2.getG(), c1.getB() - c2.getB());
}


// -
Color operator * (const Color& c1, const double mult) {
  double d = 1/mult;
  return Color(c1.getR()/d, c1.getG() /d, c1.getB() /d);
}


// -
Color operator / (const Color& c1, const double d) {
  return Color(c1.getR()/d, c1.getG() /d, c1.getB() /d);
}
