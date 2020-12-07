#include <cmath> // seno

#include "color.hpp"
#include "utils.hpp"

Color::Color() : Color(0,0,0)
{}


Color::Color(const double rgb) : Color(rgb,rgb,rgb)
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

void Color::setRandom(const GeneradorAleatorio& gen) {
  setRGB(gen.rand01(), gen.rand01(), gen.rand01());
}


void Color::setRandom() {
  GeneradorAleatorio gen;
  setRandom(gen);
  //setRGB(gen.rand01(), gen.rand01(), gen.rand01());
}



// Color para una normal (o cualquier vector normalizado)
void Color::setFromNormal(const Vector3& normal) {
  // if (normal[0]!= normal[1] && normal[1]!=normal[2]) {
  //   std::cout << "Normal: " << normal << "\nabs(normal[0]) = " << abs(normal[0]) << std::endl;
  // }
  setRGB(std::abs(normal[0]), std::abs(normal[1]), std::abs(normal[2]));
  // Vector3 normal01 = (-normal + 1.0)/ 2.0;
  // // Vector3 normal01 = normal * normal;
  // normal01 = dividirComponentes(Vector3(1,1,1,false), normal01);
  // setRGB((normal01[0]), (normal01[1]), (normal01[2]));
  // setRGB(std::abs((normal[0])/2.0+1.0), std::abs((normal[1])/2.0+1.0), std::abs((normal[2])/2.0+1.0));
}


void Color::setFromNormalVector(const Vector3& normal) {
  // std::cout << "normal: "<< normal << '\n';
  // setRGB((normal[0]+1.0)/2.0 - (normal[1]-1.0)/2.0, (normal[1]+1.0)/2.0 - (normal[2]-1.0)/2.0, (normal[2]+1.0)/2.0 - (normal[0]-1.0)/2.0);
  setRGB((normal[0]+1.0)/2.0, (normal[1]+1.0)/2.0, (normal[2]+1.0)/2.0);
}



void Color::setFromDistancia(const double t, const double min, const double max) {
  double c = (max-t) / (max-min);
  setRGB(c,c,c);
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

void Color::clamp(const float max) {
  if (r>max) r = max;
  if (g>max) g = max;
  if (b>max) b = max;
}


std::string Color::to_string() const {
  std::string s = std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + "\n";
  return s;
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

void Color::setRGB(const double rgb) {
  setRGB(rgb, rgb, rgb);
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

double Color::getPromedio() const {
  double prom = r + g + b;
  return prom/3.0;
}


// +
Color operator + (const Color& c1, const Color& c2) {
  return Color(c1.getR()+c2.getR(), c1.getG() + c2.getG(), c1.getB() + c2.getB());
}

// -
Color operator - (const Color& c1, const Color& c2) {
  return Color(c1.getR()-c2.getR(), c1.getG() - c2.getG(), c1.getB() - c2.getB());
}

bool operator == (const Color& c1, const double val) {
  return c1.getR() == c1.getG() && c1.getG()==c1.getB() && c1.getB()==val;
}


bool operator >= (const Color& c1, const Color& c2) {
  return c1.getR() >= c2.getR() && c1.getG() >= c2.getG() && c1.getB() >= c2.getB();
}

// -
Color operator * (const Color& c1, const double mult) {
  double d = 1/mult;
  return Color(c1.getR()/d, c1.getG() /d, c1.getB() /d);
}


// -
Color operator * (const double mult, const Color& c1) {
  double d = 1/mult;
  return Color(c1.getR()/d, c1.getG() /d, c1.getB() /d);
}


Color operator * (const Color& c1, const Color& c2) {
  return Color(c1[0]*c2[0], c1[1]*c2[1],c1[2]*c2[2]);
}


// -
Color operator / (const Color& c1, const double d) {
  return Color(c1.getR()/d, c1.getG() /d, c1.getB() /d);
}
