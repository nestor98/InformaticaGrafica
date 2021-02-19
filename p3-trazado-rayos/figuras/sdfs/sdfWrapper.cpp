#include <iostream>
#include <string>


#include "sdfWrapper.hpp"
//
// #include "src/primitives/sphere.hpp"
#include "src/sphere-marcher/sphere-marcher.hpp"

SDFWrapper::SDFWrapper(std::shared_ptr<SDF> _sdf) : sdf(_sdf) {}


std::optional<Figura::InterseccionData> SDFWrapper::interseccion(const Vector3& origen, const Vector3& dir) const
{
	//std::cout << "intersectando wrapper" << '\n';
	std::optional<rayMarching::Intersection> interseccion = rayMarching::sphereTrace(origen.toArray(), dir.toArray(), *sdf);
	if (!interseccion) return {};
	return InterseccionData{interseccion->distance, Vector3(interseccion->point.toArray())};
}

Vector3 SDFWrapper::getNormal(const Vector3& pto) const {
	return sdf->normal(pto.toArray());
}


std::string SDFWrapper::to_string() const {
	return "Soy una figura con SDF";
}

// De momento devuelve caja infinita, como los planos
std::shared_ptr<Prisma> SDFWrapper::getBoundingBox() const {
	return std::make_shared<CajaInfinita>(CajaInfinita());
}
