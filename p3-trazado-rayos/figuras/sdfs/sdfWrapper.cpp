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
	std::optional<rayMarching::Intersection> interseccion = rayMarching::newtonTrace(origen.toArray(), normalizar(dir).toArray(), *sdf);
	if (!interseccion || interseccion->t<=0) return {};
	Vector3 pto(interseccion->point.toArray());
	pto.setPunto();
	return InterseccionData{interseccion->t, pto};
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
