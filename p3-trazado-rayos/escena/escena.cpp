#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include <chrono> // tests

#include "escena.hpp"
#include "material.hpp"

#define hrc std::chrono::high_resolution_clock

Escena::Escena(const std::shared_ptr<Camara> _c)
: c(_c)
{
}

void Escena::addFigura(const std::shared_ptr<Figura> f)
{
	figuras.push_back(f);
}

void Escena::addFiguras(const std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectFiguras) {
	figuras.insert(figuras.end(), vectFiguras->begin(), vectFiguras->end());
}



std::string Escena::to_string() const {
	std::string s = "camara:\n" + c->to_string() + "\nfiguras:";
	if (figuras.size()>50) {
		s+= "Hay " + std::to_string(figuras.size()) +" figuras, no las listo todas\n";
	}
	else {
		for (auto f : figuras) {
			s += "\n" + f->to_string();
		}
	}
	return s;
}

/******************** Interseccion sin bvh *************************/
bool tMenor (const double t, const double min) {
	return (t<min || min==-1);
}

bool tMenor (const Figura::InterseccionData& iData, const double min) {
	return tMenor(iData.t, min);
}

// Interseccion sin bvh:
std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> Escena::interseccion(const Vector3& o, const Vector3& dir) const {
	float t = -1;
	Vector3 pto;
	std::shared_ptr<Figura> f;
	bool intersectado = false;
	for (auto fig : figuras) {
		auto iFig = fig->interseccion(o, dir);
		// std::cout << "Probando interseccion con " <<fig << '\n';
		if (iFig) {
			if (tMenor(iFig->t, t)) {
				t = iFig->t;
				pto = iFig->punto;
				f = fig;
				intersectado = true;
			}
		}

	}
	if (!intersectado) return {}; // std::nullopt
	return std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>(Figura::InterseccionData{t, pto}, f);
}



std::shared_ptr<Camara> Escena::getCamara() const {
	return c;
}


void Escena::getFiguras(std::vector<std::shared_ptr<Figura>>& v) const {
	v = figuras;
}


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Escena& e) {
	os << e.to_string();
	return os;
}
