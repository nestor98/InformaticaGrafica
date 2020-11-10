

class Color {
  double r, g, b;
public:

  Color(const double _r, const double _g, const double _b);

  Color(const std::array<3,double> rgb);

  // Da un color para un pto, negro en min, blanco en max
  // r primera coordenada, g segunda, b tercera
  Color(const Vector3& pto, const Vector3& min, const Vector3& max);

  std::array<3,double> toArray() const;

  double getR() const;
  double getG() const;
  double getB() const;

  void setRandom();

  void setR(const double r);
  void setG(const double g);
  void setB(const double b);


  void setRGB(const std::array<3,double> rgb);

  void setIndex(const int i, const double val);

}
