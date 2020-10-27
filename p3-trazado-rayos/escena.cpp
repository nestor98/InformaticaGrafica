#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include "escena.hpp"


Escena::Escena(const Camara& _c) : c(_c.getPos(), _c.getDir())
{
	//std::cout << "Constructor de escena: " << c.to_string() << std::endl;
}

void Escena::addFigura(const std::shared_ptr<Figura> f)
{
	figuras.push_back(f);
}



std::string Escena::to_string() const {
	std::string s = "camara:\n" + c.to_string() + "\nfiguras:";
	for (auto f : figuras) {
		s += "\n" + f->to_string();
	}
	return s;
}
// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Escena& e) {
	os << e.to_string();
	return os;
}
