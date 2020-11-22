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
{
	setMaximos();
}

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
Vector3 Material::getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen, const int evento,const bool inside, const Vector3& incidente) const {
	float nada;
	return getVectorSalida(base, gen, evento, inside, incidente, nada);
}



// base = T en las diapos
Vector3 Material::getVectorSalida(const Matriz4& base, const GeneradorAleatorio& gen, const int evento,const bool inside, const Vector3& incidente, float& kr) const {
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
		// Vector3 u=base.inversa()*(incidente); //inversa base* normal
		// wi=u;
		// wi[2]=-u[2];
		// wi=base*wi;
		wi = reflejar(incidente, base);

	}
	else {	//refraccion   https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
		//double coefRefraccion = 1.45; // TODO: campo del material o algo
		// Vector3 wo = base.inversa()*normalizar(incidente);
		// // std::cout << "wo: " << wo << '\n';
		// wi = wo;
		// wi[1] = coefRefraccion * wo[1];
		// wi[2] = (1.0/coefRefraccion) * wo[2];
		// std::cout << "wi: " << wi << '\n';
		// if (wo.getModulo() != wi.getModulo()) {
		// 	std::cerr << "MALLLLLLL " << wo.getModulo() << " " << wi.getModulo() << '\n';
		// }
    //float cosi = base[2]*incidente;
    //wi = (incidente * coefRefraccion - base[2] * (-cosi + coefRefraccion * cosi));
		//wi=base*wi;
		// std::cout << "wi mundo: " << wi << '\n';

		// ------------------ Fresnel:
		// Vec3f refractionColor = 0;
		// compute fresnel
		float coefRefraccion = 1.45; // TODO: de momento es vidrio
		kr = krFresnel(incidente, base[2], coefRefraccion); // Devuelve la probabilidad de reflexion
		// if (kr>0.9) std::cout << "kr:" << kr << '\n';
		if (gen.rand01() < kr) { // Reflexion:
			// Matriz4 basePrueba = base;
			// basePrueba[2] = -base[2];
			wi = reflejar(incidente, base);
		}
		else { // Refraccion:
			// std::cout << "reflexion" << '\n';
			wi = refraccion(incidente, base[2], coefRefraccion);
			kr = 1-kr; // Se devuelve kt
				// std::cout << "refraccion" << '\n';
				// float coefRefraccion = 1.45;
				// float eta = (inside) ? coefRefraccion : 1.0 / coefRefraccion; // are we inside or outside the surface? //pasar inside
		    // float cosi = -base[2]*incidente;
		    // float k = 1.0 - eta * eta * (1.0 - cosi * cosi);
		    // wi = incidente * eta + base[2] * (eta *  cosi - sqrt(k));

		}
		//
		//
		// bool outside = dir.dotProduct(hitNormal) < 0;
		// Vec3f bias = options.bias * hitNormal;
		// // compute refraction if it is not a case of total internal reflection
		// if (kr < 1) {
		// 		Vec3f refractionDirection = refract(dir, hitNormal, isect.hitObject->ior).normalize();
		// 		Vec3f refractionRayOrig = outside ? hitPoint - bias : hitPoint + bias;
		// 		refractionColor = castRay(refractionRayOrig, refractionDirection, objects, lights, options, depth + 1);
		// }
		//
		// Vec3f reflectionDirection = reflect(dir, hitNormal).normalize();
		// Vec3f reflectionRayOrig = outside ? hitPoint + bias : hitPoint - bias;
		// Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1);
		//
		// // mix the two
		// hitColor += reflectionColor * kr + refractionColor * (1 - kr);



	}
	return normalizar(wi);
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

//
// // Le cambia el color en funcion de la posicion
// void setColorFromPos(const Vector3& pto, const Vector3& min, const Vector3& max);


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Material& c);

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Material> c);
