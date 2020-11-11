
#include "utils.hpp"
#include "color.hpp"
#include "material.hpp"

bool Material::coeficientesCorrectos() const {
	for (int i = 0; i<3; i++) {
		float suma = 0;
		for (int j = 0; j<3; j++) {
			suma += coeficientes[j][i];
		}
		if (suma>0.9) return false;
	}
	return true;
}


Material::Material(bool aleatorio) {
	if (aleatorio) {
		setRandom();
	}
}

Material::Material(const Tipo predeterminado) {
	if (predeterminado==Tipo::Plastico) {
		// TODO
	}
	else {
		// TODO
	}
}


std::string Material::to_string() const {
	std::string s = "Material con coeficientes (lambertiano, reflexion, refraccion)\n";
	// for (int i = 0; i<3; i++) { // TODO:implementar en color si eso
	// 	s+= coeficientes[i].to_string() + ";\t";
	// }
	return s;

}

std::array<Color, 3> Material::getCoeficientes() const {
	return coeficientes;
}

void Material::setCoeficiente(const Color& coef, const int i) {
	coeficientes[i] = coef;
}

Color Material::getCoeficiente(const int i) const {
	return coeficientes[i];
}

void Material::setCoeficientes(const std::array<Color, 3>& _coefs) {
	coeficientes = _coefs;
}

void Material::setRandom() {
	bool fin = false;
	while (!fin) {
		for (int i = 0; i<3; i++) {
			coeficientes[i].setRandom();
		}
		fin = coeficientesCorrectos();
	}
}
//
// // Le cambia el color en funcion de la posicion
// void setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max);


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Material& c);

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Material> c);
