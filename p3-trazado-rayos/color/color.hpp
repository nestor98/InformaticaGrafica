#pragma once
#include <array>
#include "Vector3.hpp"
// #include "figura.hpp"
#include "utils.hpp"


class Color {
  double r, g, b;

  inline static Utils utils; // random, etc
public:
  Color();
  Color(const double _r, const double _g, const double _b);

  Color(const std::array<double,3> rgb);

  // Da un color para un pto, negro en min, blanco en max
  // r primera coordenada, g segunda, b tercera
  Color(const Vector3& pto, const Vector3& min, const Vector3& max);

  std::array<double,3> toArray() const;

  double getR() const;
  double getG() const;
  double getB() const;

  void setRandom();

  void setR(const double r);
  void setG(const double g);
  void setB(const double b);

  void setRGB(const double _r, const double _g, const double _b);


  void setRGB(const std::array<double,3> rgb);

  void setFromPosGrad(const Vector3& pto, const Vector3& min, const Vector3& max);
  void setFromPosExp(const Vector3& pto, const Vector3& min, const Vector3& max);

	// componente (get, a = v[2])
	double operator [](int i) const;

	// componente (set, v[2] = 3)
	double& operator [](int i);

};

// +
Color operator + (const Color& c1, const Color& c2);

// -
Color operator - (const Color& c1, const Color& c2);


// -
Color operator * (const Color& c1, const double mult);


// -
Color operator / (const Color& c1, const double d);
