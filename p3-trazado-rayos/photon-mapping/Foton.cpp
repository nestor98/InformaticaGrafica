
#include "Foton.hpp"


Foton::Foton(const Vector3& _pos, const Vector3& _dir, const Color& _e)
: pos(_pos), dir(_dir), e(_e)
{

}

Vector3 Foton::getPos() const {
  return pos;
}

Color Foton::getEmision() const {
  return e;
}
