#pragma once
#include <array>
#include "Vector3.hpp"
// #include "figura.hpp"
#include "utils.hpp"


class Color {
  double r, g, b;

  //inline static Utils utils; // random, etc
public:
  Color();
  Color(const double rgb); // pone las tres a ese color
  Color(const double _r, const double _g, const double _b);

  Color(const std::array<double,3> rgb);

  // Da un color para un pto, negro en min, blanco en max
  // r primera coordenada, g segunda, b tercera
  Color(const Vector3& pto, const Vector3& min, const Vector3& max);

  std::array<double,3> toArray() const;

  std::string to_string() const;

  double getR() const;
  double getG() const;
  double getB() const;

  // Color aleatorio:
  void setRandom(); // mas comoda
  void setRandom(const GeneradorAleatorio& gen); // mas aleatoria (y rapida)

  void setR(const double r);
  void setG(const double g);
  void setB(const double b);

  void setRGB(const double _r, const double _g, const double _b);

  void clamp(const float max);


  void setRGB(const std::array<double,3> rgb);

  void setFromPosGrad(const Vector3& pto, const Vector3& min, const Vector3& max);
  void setFromPosExp(const Vector3& pto, const Vector3& min, const Vector3& max);

  void setFromNormal(const Vector3& normal);
  void setFromNormalNoAbs(const Vector3& normal);

  void setFromDistancia(const double t, const double min = 0, const double max = 4);

	// componente (get, a = v[2])
	double operator [](int i) const;

	// componente (set, v[2] = 3)
	double& operator [](int i);

};

bool operator == (const Color& c1, const double val);

// +
Color operator + (const Color& c1, const Color& c2);

// -
Color operator - (const Color& c1, const Color& c2);


// -
Color operator * (const Color& c1, const Color& c2);


// -
Color operator * (const Color& c1, const double mult);


// -
Color operator / (const Color& c1, const double d);
