#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
// cmath para sqrt

#include "camara.hpp"
#include "figura.hpp"

#include "utils.hpp"

class Escena {
	std::shared_ptr<Camara> c;
	std::vector<std::shared_ptr<Figura>> figuras;

	Utils utils;
public:
	Escena(const Camara& _c);
	Escena(const std::shared_ptr<Camara> _c);

	std::string to_string() const;
	void addFigura(const std::shared_ptr<Figura> f);

	void render(const std::string fichero) const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Escena& e);
