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

Material::Material(const float& coef) : Material(Material::Difuso, coef)
{
	setMaximos();
}

Material::Material(const Color& c1, const Color& c2, const Color& c3, const float& coef):	coeficienteRefraccion(coef)

{
	coeficientes[0]=(c1);
	coeficientes[1]=(c2);
	coeficientes[2]=(c3);
	setMaximos();
}



bool Material::esDelta(const int evento) const {
	return evento==1||evento==2;
}

// Material::Material(bool aleatorio) {
// 	if (aleatorio) {
// 		setRandom();
// 		setMaximos();
// 	}
// }

Material::Material(const Tipo predeterminado, const float& coef):coeficienteRefraccion(coef) {
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
	}else if(predeterminado==Tipo::Especular){
		coeficientes[1].setRGB(0.9,0.9,0.9);
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
				if (maxCoefs[i]>0.9) {
					std::cerr << "El coeficiente maximo no puede ser mayor de 0.9!!" << '\n';
					exit(1);
				}
			}
		}
	}
}

// Devuelve un kr (porcentaje de reflexion) en 0..1, representa la proporción de energia que se reflecta
// Tb adaptado de https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
double Material::krFresnel(const Vector3& normal, const Vector3& wo, const double coefRefraccion) const {
  float cosI = wo * normal; //clamp(-1, 1, dotProduct(I, N));
	if (cosI < -1.0) cosI = -1.0;
	else if (cosI > 1.0) cosI = 1.0;
  float cRefrMedio1 = 1;
	float cRefrMedio2 = coefRefraccion;
  if (cosI > 0) { // Esta saliendo del medio, no entrando
		std::swap(cRefrMedio1, cRefrMedio2);
	}
  // Compute sini using Snell's law
	float cosIsq = cosI*cosI;
	float senoT = 0;
	if (cosIsq >= 1.0) {
		senoT = cRefrMedio1 / cRefrMedio2 * sqrt(1 - cosIsq);
	}
  if (senoT >= 1) { // Mas del angulo critico: reflexion interna total
		std::cout << "reflexion interna total" << '\n';
    return 1;
  }
  else {
    float cosT = sqrtf(std::max(0.f, 1 - senoT * senoT));
    cosI = std::abs(cosI);
    float Rs = ((cRefrMedio2 * cosI) - (cRefrMedio1 * cosT)) / ((cRefrMedio2 * cosI) + (cRefrMedio1 * cosT));
    float Rp = ((cRefrMedio1 * cosI) - (cRefrMedio2 * cosT)) / ((cRefrMedio1 * cosI) + (cRefrMedio2 * cosT));
    return (Rs * Rs + Rp * Rp) / 2;
  }
}

Vector3 refraccion(const Vector3& incidente, const Vector3& normal, const float indice)
{
	float cosI = incidente * normal; //clamp(-1, 1, dotProduct(I, N));
	if (cosI < -1.0) cosI = -1.0;
	else if (cosI > 1.0) cosI = 1.0;
  float etai = 1, etat = indice;
  Vector3 n = normal;
  if (cosI < 0) {
		cosI = -cosI;
	}
	else {
		std::swap(etai, etat);
		n = -normal;
	}
  float eta = etai / etat;
  float k = 1 - eta * eta * (1 - cosI * cosI);
  return k < 0 ? 0 : eta * incidente + (eta * cosI - sqrt(k)) * n;
}


// base = T en las diapos
Vector3 Material::getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen, const int evento, const Vector3& incidente) const {
	float nada;
	return getVectorSalida(base, gen, evento, incidente, nada);
}



// base = T en las diapos
Vector3 Material::getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen, const int evento, const Vector3& incidente, float& kr) const {
	Vector3 wi;
	if (evento==0) { //difuso
		double rand1 = gen.rand01();
		double rand2 = gen.rand01();
		double theta = acos(sqrt(1.0-rand1));
		double phi = 2.0 * PI * rand2;
		wi = base * Vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta), false);
	}
	else if(evento==1){		//especular
		wi = reflejar(incidente, base);
	}
	else {	//refraccion   https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
		// ------------------ Fresnel:
		// Vec3f refractionColor = 0;
		// compute fresnel
		float coefRefraccion = coeficienteRefraccion; // TODO: de momento es vidrio
		kr = krFresnel(incidente, base[2], coeficienteRefraccion); // Devuelve la probabilidad de reflexion
		if (gen.rand01() < kr) { // Reflexion:
			wi = reflejar(incidente, base);
		}
		else { // Refraccion:
			// std::cout << "reflexion" << '\n';
			wi = refraccion(incidente, base[2], coeficienteRefraccion);
			if (wi.getModulo() == 0) { // Reflexion interna total
				wi = reflejar(incidente, base);
			} else {
				kr = 1-kr; // Se devuelve kt
			}
		}
	}
	try {
		return normalizar(wi);
	}
	catch (std::string e) {
		if (evento == 2) {
			std::cerr << "Error normalizando: " << e << '\n';
			std::cerr << "EN [DIFUSO, REFLEXION, REFRACCION]: "<<evento  << '\n';
			std::cerr << "wi:" << wi << '\n';
			std::cerr << "Incidente: " << incidente << '\n';
			std::cerr << "base: " << base << '\n';
		}

	}
}


std::string Material::to_string() const {
	std::string s = "Material con coeficientes (lambertiano, reflexion, refraccion)\n";
	for (int i = 0; i<3; i++) { // TODO:implementar en color si eso
		s+= coeficientes[i].to_string() + ";\t";
	}
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

float Material::getPDF(const int evento, const bool primerRebote) const {
	float pdf = 0;
	if (0 <= evento || evento < 3) {
		if (!primerRebote) {
			return maxCoefs[evento];
		}
		else { // Primer rebote
			double totalCoefs = maxCoefs[0]+maxCoefs[1]+maxCoefs[2];
			return maxCoefs[evento]/totalCoefs; // los 0,9 deben ser 1, etc.
		}
	}
	else {
		if (primerRebote) {
			std::cout << "QUE: " << evento << " " << primerRebote << '\n';
			return 0;
		}
		else return 1-std::accumulate(maxCoefs.begin(), maxCoefs.end(), 0.0);
	}

}


	// devuelve un num de 0 a 4 (para difuso, especular, refraccion o absorcion, respectivamente)
	// con probabilidades en funcion del coeficiente máximo de cada uno
	int Material::ruletaRusa(const GeneradorAleatorio& gen, const bool primerRebote) const {
		double random = gen.rand01();
		double totalCoefs = maxCoefs[0]+maxCoefs[1]+maxCoefs[2];
		while (primerRebote && random >= totalCoefs) { // en el primer rebote no puede absorber
			random -= 1 - totalCoefs;
		}
		//std::cout << "maxCoefs: " << maxCoefs[0] << " " << maxCoefs[1] << " "<< maxCoefs[2] << '\n';
		if (random < maxCoefs[0]) {
			return 0; // Difuso
		}
		else if (random < maxCoefs[0]+maxCoefs[1]) {
			return 1; // Reflexion
		}
		else if (random < maxCoefs[0]+maxCoefs[1]+maxCoefs[2]) {
			// std::cout << "hola" << '\n';
			return 2; // Refraccion
		}
		else {
			//std::cout << "Absorcion: " << random << '\n';
			return 3; // Absorcion
		}
	}

	// int Material::ruletaRusaPM(const GeneradorAleatorio& gen, const bool primerRebote) const {
	// 	double random = gen.rand01();



//
// // Le cambia el color en funcion de la posicion
// void setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max);


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Material& c) {
	os << c.to_string();
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Material> c){
	os << c->to_string();
	return os;
}
