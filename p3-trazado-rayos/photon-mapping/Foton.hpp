#pragma once


#include "Vector3.hpp"
#include "color.hpp"

class Foton {
  Vector3 pos;
  Vector3 dir;
  Color e;
public:
  Foton();
  Foton(const Vector3& _pos, const Vector3& _dir, const Color& _e);

  Vector3 getPos() const;
  Color getEmision() const;
  Vector3 getDir() const;

};
