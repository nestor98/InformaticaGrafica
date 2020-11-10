
Color::Color() : Color(0,0,0)
{}

Color::Color(const double _r, const double _g, const double _b)
: r(_r), g(_g), b(_b)
{}

Color::Color(const std::array<3,double> rgb) : Color(rgb[0], rgb[1], rgb[2])
{}

// Da un color para un pto, negro en min, blanco en max
// r primera coordenada, g segunda, b tercera
Color::Color(const Vector3& pto, const Vector3& min, const Vector3& max)
{

}

void setRandom() {
  setColor(utils->rand01(), utils->rand01(), utils->rand01());
}


std::array<3,double> Color::toArray() const {
  return std::array<3,double>{r,g,b};
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

void Color::setRGB(const std::array<3,double> rgb) {
  setRGB(rgb[0], rgb[1], rgb[2]);
}

void Color::setIndex(const int i, const double val) {
  switch (i) {
    case 0: r = val;
    break;
    case 1: g = val;
    break;
    default: b = val;
  }
}
