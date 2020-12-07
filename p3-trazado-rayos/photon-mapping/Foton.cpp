
#include "Foton.hpp"


Foton::Foton()
: pos(0), dir(0), e(0)
{
}

Foton::Foton(const Vector3& _pos, const Vector3& _dir, const Color& _e)
: pos(_pos), dir(_dir), e(_e)
{

}

Vector3 Foton::getPos() const {
  return pos;
}

Vector3 Foton::getDir() const {
  return dir;
}

Color Foton::getEmision() const {
  return e;
}
