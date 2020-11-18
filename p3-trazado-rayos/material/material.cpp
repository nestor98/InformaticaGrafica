#include <cmath>
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

Material::Material() : Material(Material::Difuso)
{}

Material::Material(const Color& c1, const Color& c2, const Color& c3)
{
	coeficientes[0]=(c1);
	coeficientes[1]=(c2);
	coeficientes[2]=(c3);
	setMaximos();
}

// Material::Material(bool aleatorio) {
// 	if (aleatorio) {
// 		setRandom();
// 		setMaximos();
// 	}
//
// }

Material::Material(const Tipo predeterminado) {
	if (predeterminado==Tipo::Plastico) {
		// TODO
		std::cerr << "TODO: tipo plastico" << '\n';
		exit(1);
	}
	else if (predeterminado==Tipo::Difuso) {
		coeficientes[0].setRGB(0.9,0.9,0.9);
		//coeficientes[0].setRandom();
		// while (!coeficientesCorrectos()) {
		// 	coeficientes[0].setRandom();
		// }
	}
	else {
		std::cerr << "TODO: otros materiales" << '\n';
		exit(1);
	}
	setMaximos();
}

void Material::setMaximos() {
	for (int i = 0; i < 3; i++) {
		maxCoefs[i] = 0;
		for (int j = 0; j < 3; j++) {
			if (coeficientes[i][j]>maxCoefs[i]) {
				maxCoefs[i] = coeficientes[i][j];
			}
		}
	}
}

// base = T en las diapos
Vector3 Material::getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen, const int evento, const Vector3& incidente) const {
	Vector3 wi;
	if (evento==0) { //difuso
		double rand1 = gen.rand01();
		double rand2 = gen.rand01();
		double theta = acos(sqrt(1.0-rand1));
		double phi = 2.0 * PI * rand2;
		// std::cout << "---------------------------------" << '\n';
		// std::cout << "theta: " << theta << "\nphi: " <<phi << '\n';
		// std::cout << "base: " << base << '\n';
		wi = base * Vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta), false);
		// std::cout << "Dcha: "<<-LEFT << '\n'<< "wi:" << wi<<'\n';
		// std::cout << "wiiiii: "<<wi << '\n';
	}else if(evento==1){		//especular
		//angulo entre dir y normal
		//double an=acos((base[3]*base[0])/(base[3].getModulo()*base[0].getModulo()));
		Vector3 u=base.inversa()*incidente; //inversa base* normal
		wi=u;
		wi[0]=-u[0];
		wi=base*wi;
	}
	else {	//refraccion

		std::cerr << "Aun no has implementado otros materiales..." << '\n';
		exit(1);
	}
	return wi;
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
	setMaximos();
}

Color Material::getCoeficiente(const int i) const {
	return coeficientes[i];
}

void Material::setCoeficientes(const std::array<Color, 3>& _coefs) {
	coeficientes = _coefs;
	setMaximos();
}

void Material::setRandom() {
	bool fin = false;
	while (!fin) {
		for (int i = 0; i<3; i++) {
			coeficientes[i].setRandom();
		}
		fin = coeficientesCorrectos();
	}
	setMaximos();
}


	// devuelve un num de 0 a 4 (para difuso, especular, refraccion o absorcion, respectivamente)
	// con probabilidades en funcion del coeficiente máximo de cada uno
	int Material::ruletaRusa(const GeneradorAleatorio& gen, const bool primerRebote) const {
		double random = gen.rand01();
		while (primerRebote && random >= 0.9) { // en el primer rebote no puede absorber
			random -= 0.1;
		}
		//std::cout << "maxCoefs: " << maxCoefs[0] << " " << maxCoefs[1] << " "<< maxCoefs[2] << '\n';
		if (random < maxCoefs[0]) {
			return 0; // Difuso
		}
		else if (random < maxCoefs[0]+maxCoefs[1]) {
			std::cerr << "Aun no has implementado reflexiones xd" << '\n';
			exit(1);
			return 1; // Reflexion
		}
		else if (random < maxCoefs[0]+maxCoefs[1]+maxCoefs[2]) {
			std::cerr << "Aun no has implementado refracciones xd" << '\n';
			exit(1);
			return 2; // Refraccion
		}
		else {
			//std::cout << "Absorcion: " << random << '\n';
			return 3; // Absorcion
		}
	}

//
// // Le cambia el color en funcion de la posicion
// void setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max);


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Material& c);

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Material> c);
