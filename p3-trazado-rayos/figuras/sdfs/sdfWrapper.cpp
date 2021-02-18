#include <iostream>
#include <string>


#include "esfera.hpp"

#include "TFG-SDF/src/primitives/sphere.hpp"
#include "TFG-SDF/src/sphere-marcher/sphere-marcher.hpp"

SDFWrapper::SDFWrapper(std::shared_ptr<SDF> _sdf) : sdf(_sdf) {}

std::string SDFWrapper::to_string() const {
	return "--- SDFWrapper:\nposicion: " + posicion.to_string() + "\nradio: " + std::to_string(radio);
}

std::optional<Figura::InterseccionData> SDFWrapper::interseccion(const Vector3& origen, const Vector3& dir) const
{
	std::optional<rayMarching::Intersection> interseccion = rayMarching::sphereTrace(origen.toArray(), dir.toArray(), *sdf);
	if (!interseccion) return {};
	return InterseccionData{interseccion.distance, interseccion.point};
}

Vector3 SDFWrapper::getNormal(const Vector3& pto) const {
	return sdf.normal(pto.toArray());
}

// De momento devuelve caja infinita, como los planos
std::shared_ptr<Prisma> SDFWrapper::getBoundingBox() const {
	return std::make_shared<CajaInfinita>(CajaInfinita());
}
