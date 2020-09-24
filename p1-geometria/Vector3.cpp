#include <iostream>
#include <array>
#include <string>
#include <cmath>
// cmath para sqrt

const int TAM_MATRIZ = 4;
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
		if (original.esVector() != this->esVector()) {
			std::cout << "???????" << std::endl;
		}
	}

	Vector3(float x, float y, float z, float homogenea) {
		c[0] = x;
		c[1] = y;
		c[2] = z;
		c[3] = homogenea;
	}

	Vector3(float x, float y, float z, bool punto) {
		c[0] = x;
		c[1] = y;
		c[2] = z;
		c[3] = punto ? 1 : 0;
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
private:
	std::array<Vector3, 4> m; // formada por 4 vectores (verticales)

	// Matriz identidad, se usa en otros metodos:
	void setIdentidad() {
		m[0] = Vector3(1, 0, 0, float(0));
		m[1] = Vector3(0, 1, 0, float(0));
		m[2] = Vector3(0, 0, 1, float(0));
		m[3] = Vector3(0, 0, 0, float(1));
	}

	// Escribe los parametros en la diagonal:
	void setDiagonal(const float d0, const float d1, const float d2, const float d3) {
		m[0][0] = d0;
		m[1][1] = d1;
		m[2][2] = d2;
		m[3][3] = d3;
	}

	// auxiliar de determinante(), saca el determinante de una parte
	float determinante(const int filaIni, const int colIni, const int tam) const {
		float det = 0;
		if (tam <= 1) { // tamaño es 1, el det es el de esa posicion
			det = m[filaIni][colIni];// TODO: otras columnas!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
		else {
			for (int i = 0; i<)
			det = m[filaIni][colIni] * det(filaIni + 1, (colIni + 1) % TAM_MATRIZ, tam - 1);
		}

	}

public:
	/**************** Constructores ****************/

	// Por defecto, la identidad (no hace nada)
	Matriz4() { 
		setIdentidad();
	}

	Matriz4(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4) {
		m[0] = v1;
		m[1] = v2;
		m[2] = v3;
		m[3] = v4;
	}

	/**************** Definiciones de transformaciones especificas ****************/

	// desplazamiento en x,y,z:
	void setTraslacion(const float x, const float y, const float z) {
		setIdentidad();
		m[3] = Vector3(x, y, z, float(1));
	}

	// escalar en sx, sy, sz:
	void setEscalar(const float sx, const float sy, const float sz) {
		setIdentidad();
		setDiagonal(sx, sy, sz, 1);
	}

	// Rotar en x
	/*
		1    0    0    0
		0   cos -sin   0
		0   sin  cos   0
		0    0    0    1
	*/
	void setRotarX(const float theta) {
		setIdentidad();
		m[1][1] = cos(theta);
		m[1][2] = sin(theta);
		m[2][1] = -sin(theta);
		m[2][2] = cos(theta);
	}

	// Rotar en y
	/*
		cos  0   sin   0
		0    1    0    0
	   -sin  0   cos   0
		0    0    0    1
	*/
	void setRotarY(const float theta) {
		setIdentidad();
		m[0][0] = cos(theta);
		m[0][2] = -sin(theta);
		m[2][0] = sin(theta);
		m[2][2] = cos(theta);
	}

	// Rotar en z
	/*
		cos -sin   0    0
		sin  cos   0    0
		 0    0    1    0
		 0    0    0    1
	 */
	void setRotarZ(const float theta) {
		setIdentidad();
		m[0][0] = cos(theta);
		m[0][1] = sin(theta);
		m[1][0] = -sin(theta);
		m[1][1] = cos(theta);
	}

	// para cambiar un vector a una nueva base
	void setCambioBase(const Vector3& eje1, const Vector3& eje2, const Vector3& eje3, const Vector3& origen) {
		// comprobaciones...
		if (!eje1.esVector() || !eje2.esVector() || !eje3.esVector()) {
			std::cerr << "Los tres primeros parametros del cambio de base deben ser vectores" << std::endl;
			exit(1);
		}
		if (origen.esVector()) {
			std::cerr << "El origen del cambio de base debe ser un punto" << std::endl;
			exit(1);
		}
		m[0] = eje1;
		m[1] = eje2;
		m[2] = eje3;
		m[3] = origen;
	}



	float determinante() const {
		// TODO
	}

	Matriz4 inversa() const {
		// TODO: implementar la inversa..........
	}

	// como string
	std::string to_string() const {
		std::string s = "";
		for (int i = 0; i < 4; i++) {
			for (int col = 0; col < 4; col++) {
				s += std::to_string(m[col][i]) + "\t";
			}
			s += "\n";
			//s += m[i].to_string() + "\n";
		}
		return s;
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

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Matriz4& m)
{
	os << m.to_string();
	return os;
}

// Comparaciones

// TODO: ahora, un vector y un punto con las mismas componentes no son iguales,
// revisar si seria mejor que lo fueran (cambiando el i<4 por i<3)
bool operator == (const Vector3& v1, const Vector3& v2) {
	bool iguales = true;
	for (int i = 0; i < 4; i++) {
		iguales &= v1[i] == v2[i];
	}
	return iguales;
}

bool operator != (const Vector3& v1, const Vector3& v2) {
	return !(v1 == v2);
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

// cross product (producto vectorial)
Vector3 cross(const Vector3& v1, const Vector3& v2) { // TODO: comprobar
	//std::cout << "v1: " << v1 << std::endl;
	//std::cout << "v2: " << v2 << std::endl;
	Vector3 res(v1[1]*v2[2] - v1[2]*v2[1], v1[0]*v2[2] - v1[2]*v2[0], v1[0]*v2[1] - v1[1]*v2[0], false);
	//std::cout << "v1 x v2 = " << res << std::endl;
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