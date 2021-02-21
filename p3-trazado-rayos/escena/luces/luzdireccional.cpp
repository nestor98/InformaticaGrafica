#include <iostream>
#include <string>

#include "figura.hpp"
#include "prisma.hpp"

#include "Vector3.hpp"

#include "luzdireccional.hpp"

LuzDireccional::LuzDireccional() : LuzDireccional(-UP+LEFT+FRONT)
{}

LuzDireccional::LuzDireccional(const Vector3& _dir, const Color& _e) :
dir(_dir), emision(_e)
{}


Vector3 LuzDireccional::getDir() const {
	return dir;
}

// Emision -------------------------
Color LuzDireccional::getEmision() const {
	return emision;
}
//Vector3 getDir() const;
void LuzDireccional::setEmision(const double r, const double g, const double b) {
	setEmision(Color(r,g,b));
}


void LuzDireccional::setEmision(const Color& e) {
	emision = e;
}
