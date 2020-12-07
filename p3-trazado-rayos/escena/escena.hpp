#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>


#include "Imagen.hpp"
#include "camara.hpp"
#include "figura.hpp"
#include "material.hpp"
#include "bvh.hpp"

#include "utils.hpp"
#include "luzpuntual.hpp"

class Escena {

	std::shared_ptr<Camara> c;
	std::vector<std::shared_ptr<Figura>> figuras;

	std::vector<LuzPuntual> luces;

public:
	// Escena(const Camara& _c, const TipoRender tipo = BVHEmision);
	Escena(const std::shared_ptr<Camara> _c);

	std::string to_string() const;
	void addFigura(const std::shared_ptr<Figura> f);
	void addLuz(const LuzPuntual& luz);

	void addFiguras(const std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectFiguras);

	std::shared_ptr<Camara> getCamara() const;
	// std::shared_ptr<std::vector<std::shared_ptr<Figura>>> getFiguras() const;

	void getFiguras(std::vector<std::shared_ptr<Figura>>& v) const;

	void getLuces(std::vector<LuzPuntual>& vLuces) const;

	int getNumLuces() const;

	void getLuz(LuzPuntual& luz, const int i) const;
	LuzPuntual getLuz(const int i) const;

	void setMaterialFiguras(const std::vector<Material>& materiales);


	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccion(const Vector3& o, const Vector3& dir) const;

	// Compara los tiempos de render (secuencial, sin threads) de la escena con
	// el metodo original y el bvh. Muestra los resultados por salida estandar.
	// void testBVHRender(const std::string f1 = "out/testRenderOriginal.ppm", const std::string f2 = "out/testRenderBVH.ppm");
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Escena& e);
