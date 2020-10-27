#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
// cmath para sqrt

#include "camara.hpp"
#include "figura.hpp"

class Escena {
	Camara c;
	std::vector<std::shared_ptr<Figura>> figuras;
public:
	Escena(const Camara& _c);

	std::string to_string() const;
	void addFigura(const std::shared_ptr<Figura> f);
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Escena& e);
