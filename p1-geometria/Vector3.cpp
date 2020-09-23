#include <iostream>
#include <array>
#include <string>
#include <cmath>
// cmath para sqrt


class Matriz4; 

class Vector3 {
	std::array<float, 4> c; // coordenadas, con coord homogenea

public:

	/**************** Constructores ****************/
	Vector3() {
	}

	Vector3(const Vector3& original) {
		for (int i = 0; i < 4; i++) {
			c[i] = original[i];
		}
	}

	Vector3(float x, float y, float z, float homogenea) {
		c[0] = x;
		c[1] = y;
		c[2] = z;
		c[4] = homogenea;
	}

	Vector3(float x, float y, float z, bool punto) {
		c[0] = x;
		c[1] = y;
		c[2] = z;
		c[4] = punto ? 1 : 0;
	}

	/**************** Metodos ****************/

	// True sii es un vector (false si es punto)
	bool esVector() const {
		return c[3] == 0;
	}

	// Representacion en string del vector
	std::string to_string() const {
		std::string s = std::to_string(c[0]);
		s += " " + std::to_string(c[1]) + " " + std::to_string(c[2]) + " " + std::to_string(c[3]);
		return s;
	}

	// Devuelve el modulo del vector
	// TODO: comprobar!!
	float getModulo() const {
		float mod = 0;
		for (int i = 0; i < 4; i++) {
			mod += c[i] * c[i]; // cada componente al cuadrado
		}
		return sqrt(mod); // raiz de la suma de los cuads
	}

	// Producto vectorial (cross product)
	Vector3 prodVectorial(const Vector3& otro) {
		return otro; // TODO: implementar
	}

	// con el otro nombre, por que no
	Vector3 cross(const Vector3& otro) {
		return prodVectorial(otro);
	}

	// Cambio de sentido
	Vector3 operator - () const {
		return Vector3(-c[0], -c[1], -c[2], false);
	}

	// componente (get, a = v[2])
	float operator [](int i) const {
		return c[i];
	}

	// componente (set, v[2] = 3) 
	float& operator [](int i) {
		return c[i];
	}

};


// Clase matriz4, para las transformaciones de vectores
class Matriz4 {
	std::array<Vector3, 4> m; // formada por 4 vectores (verticales)

public:
	Matriz4(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4) {
		m[0] = v1;
		m[1] = v2;
		m[2] = v3;
		m[3] = v4;
	}

	// Devuelve la iesima fila como Vector3
	Vector3 fila(const int i) const {
		Vector3 vFila(m[0][i], m[1][i], m[2][i], m[3][i]);
		return vFila;
	}

	// m[i] devuelve la iesima columna como Vector3
	Vector3 operator [](int i) const {
		return m[i];
	}


};

/**************** OPERADORES ****************/

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << v.to_string();
	return os;
}

// +
Vector3 operator + (const Vector3& v1, const Vector3& v2) {
	Vector3 res(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2], false);
	return res;
}

// -
Vector3 operator - (const Vector3& v1, const Vector3& v2) {
	Vector3 res(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2], false);
	return res;
}

// escalar s*v
Vector3 operator * (const float& s, const Vector3& v) {
	Vector3 res(v[0] * s, v[1] * s, v[2] * s, false);
	return res;
}

// escalar s*v (orden v*s)
Vector3 operator * (const Vector3& v, const float& s) {
	Vector3 res(v[0] * s, v[1] * s, v[2] * s, false);
	return res;
}


// escalar v/s 
Vector3 operator / (const Vector3& v, const float& s) {
	if (s == 0) {
		std::cerr << "no dividas entre 0..." << std::endl;
		exit(1);
	}
	Vector3 res(v[0] / s, v[1] / s, v[2] / s, false);
	return res;
}

// Prod escalar (dot product)
float operator * (const Vector3& v1, const Vector3& v2) {
	float res = 0;
	for (int i = 0; i < 4; i++) {
		res += v1[i] * v2[i];
	}
	return res;
}

// Producto M*v (transformaciones, cambios de base...). Devuelve otro Vector3
Vector3 operator * (const Matriz4& m, const Vector3& v) {
	Vector3 res;
	for (int i = 0; i < 4; i++) {
		res[i] = v * m.fila(i); // prod escalar del vector y la fila
	}
	return res;
}