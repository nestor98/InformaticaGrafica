#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

#include "renderer.hpp"

class RayMarcher : public Renderer {

protected:
	TipoRender renderSeleccionado;

public:
	// RayMarcher(const Camara& _c, const TipoRender tipo = BVHEmision);
	RayMarcher(const int _nThreads = 12, const TipoRender tipo = Materiales, const bool _usarBVH = true);
	RayMarcher(const Escena& _e, const int _nThreads = 12, const TipoRender tipo = Materiales, const bool _usarBVH = true, const float _rangoDinamico=20);

	std::string to_string() const;

	void render(const std::string fichero);
	//

};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const RayMarcher& e);
