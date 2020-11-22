#include <iostream>
#include <string>

#include "figura.hpp"
#include "prisma.hpp"

#include "Vector3.hpp"

#include "luzpuntual.hpp"

LuzPuntual::LuzPuntual() : LuzPuntual(Vector3())
{}

LuzPuntual::LuzPuntual(const Vector3& _pos, const Color& _e) :
pos(_pos), emision(_e)
{}

std::string LuzPuntual::to_string() const {
	return "no esta implementao";
}

Vector3 LuzPuntual::getPos() const {
	return pos;
}

// Emision -------------------------
Color LuzPuntual::getEmision() const {
	return emision;
}
//Vector3 getDir() const;
void LuzPuntual::setEmision(const double r, const double g, const double b) {
	setEmision(Color(r,g,b));
}


void LuzPuntual::setEmision(const Color& e) {
	emision = e;
}
