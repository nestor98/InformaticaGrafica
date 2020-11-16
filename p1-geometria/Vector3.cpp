#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <list>
// cmath para sqrt
#include "Vector3.hpp"


Vector3::Vector3() : Vector3(1,1,1,false)
{}

Vector3::Vector3(const float tam) : Vector3(tam, tam, tam, false)
{}

Vector3::Vector3(const Vector3& original) {
	for (int i = 0; i < 4; i++) {
		c[i] = original[i];
	}
	//std::cout<<"copiado " << original << "\ncopia: " << to_string() << std::endl;
	if (original.esVector() != this->esVector()) {
		std::cout << "???????" << std::endl;
	}
}

Vector3::Vector3(float x, float y, float z, float homogenea) {
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = homogenea;
}

Vector3::Vector3(float x, float y, float z, bool punto) {
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = punto ? 1 : 0;
}

/**************** Metodos ****************/


void Vector3::setCoords(std::array<float, 3> _c) {
	setCoords(_c[0], _c[1], _c[2], 0);
}

void Vector3::setCoords(float x, float y, float z, float homogenea) {
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = homogenea;
}

// True sii es un vector (false si es punto)
bool Vector3::esVector() const {
	return c[3] == 0;
}

// True sii todos sus componentes son >=0
bool Vector3::esPositivo() const {
	bool pos = true;
	for (int i = 0; i<3; i++) {
		pos &= c[i]>=-0.001;
	}
	return pos;
}

// Representacion en string del vector
std::string Vector3::to_string() const {
	std::string s = std::to_string(c[0]);
	s += " " + std::to_string(c[1]) + " " + std::to_string(c[2]) + " " + std::to_string(c[3]);
	return s;
}

// Devuelve el modulo del vector
// TODO: comprobar!!
float Vector3::getModulo() const {
	return sqrt(getModuloSq()); // raiz de la suma de los cuads
}

// Devuelve el modulo del vector
// TODO: comprobar!!
float Vector3::setModulo(const float mod) {
	Vector3 temp = mod*((*this)/getModulo());
	setCoords(temp[0], temp[1], temp[2], 0);
}

void Vector3::normalizar() {
	std::cerr << "HOLA SOY Vector3::normalizar() Y NO FUNCIONO, USA normalizar(Vector3) O ARREGLAME\n";
	exit(1);
	// No funciona:
	float mod = getModulo();
	c[0]/mod; c[1]/mod; c[2]/mod;
}

// Devuelve el vector v normalizado:
Vector3 normalizar(const Vector3& v) {
	//Vector3 normalizado = v / v.getModulo();
	return v/v.getModulo();
}

// devuelve el modulo al cuadrado
float Vector3::getModuloSq() const {
	float mod = 0;
	for (int i = 0; i < 3; i++) { // TODO: hasta 3 o 4????
		mod += c[i] * c[i]; // cada componente al cuadrado
	}
	return mod;
}

// Cambio de sentido
Vector3 Vector3::operator - () const {
	return Vector3(-c[0], -c[1], -c[2], -c[3]);
}

// componente (get, a = v[2])
float Vector3::operator [](int i) const {
	return c[i];
}

// componente (set, v[2] = 3)
float& Vector3::operator [](int i) {
	return c[i];
}



// Clase matriz4, para las transformaciones de vectores

// Matriz identidad, se usa en otros metodos:
void Matriz4::setIdentidad() {
	m[0] = Vector3(1, 0, 0, float(0));
	m[1] = Vector3(0, 1, 0, float(0));
	m[2] = Vector3(0, 0, 1, float(0));
	m[3] = Vector3(0, 0, 0, float(1));
}

// Escribe los parametros en la diagonal:
void Matriz4::setDiagonal(const float d0, const float d1, const float d2, const float d3) {
	m[0][0] = d0;
	m[1][1] = d1;
	m[2][2] = d2;
	m[3][3] = d3;
}


// auxiliar de determinante(), saca el determinante de una submatriz
// compuesta por las columnas con indice <columnas> y filas de indice <filas>
float Matriz4::determinante(const std::list<int> &columnas, const std::list<int> &filas) const {
	float det = 0;
	int tam = columnas.size();
	int s = 1; // signo
	if (tam != filas.size()) {
		std::cerr << "La matriz del determinante debe ser cuadrada" << std::endl;
		exit(1);
	}
	for (auto col : columnas) {
		if (tam == 1) {
			det = m[col][filas.front()];
		}
		else {
			// Nuevas columnas:
			auto columnasRed(columnas);
			columnasRed.remove(col);

			// Nuevas filas:
			auto filasRed(filas);
			filasRed.remove(filas.front());

			det += s * m[col][filas.front()] * determinante(columnasRed, filasRed);
			s *= -1; // cambio de signo
		}
	}
	//if (tam >= 1)
		//std::cout << "FIN determinante " << colIni << "," << filaIni << " " << tam << " -> det = " << det<< std::endl;
	return det;
}

std::string str(std::list<int> l) {
	std::string s = "";
	for (auto e : l) {
		s += e + ",";
	}
	return s;
}

// devuelve la transpuesta de la matriz de cofactores (cada elto, el determinante de las otras filas y cols
// * signo + - + -...)
Matriz4 Matriz4::adj() const {
	Matriz4 res;
	int s; // signo
	std::list<int> cols, filas, todas;
	todas = {0,1,2,3};
	for (int col = 0; col < TAM_MATRIZ; col++) {
		for (int fila = 0; fila < TAM_MATRIZ; fila++) {
			cols = todas; cols.remove(col); // todas las cols menos la actual
			filas = todas; filas.remove(fila); // todas las filas menos la actual
			/*std::cout << "------------------\n"<< col << "," << fila << std::endl;
			for (auto e:cols) {
				std::cout << e << ";";
			}
			std::cout << std::endl;
			for (auto e:filas) {
				std::cout << e << ";";
			}
			std::cout << std::endl;
			*/
			s = (fila+col) % 2 == 0 ? 1 : -1;
			float det = determinante(cols, filas);
			res[fila][col] = s * determinante(cols, filas); // indices al reves para traspuesta
			//std::cout << res[fila][col] << ".........." << det << std::endl;
		}
	}
	//std::cout << "adj dentro de la funcion\n";
	//std::cout << res << std::endl;
	return res;
}



/**************** Constructores ****************/

// Por defecto, la identidad (no hace nada)
Matriz4::Matriz4() {
	setIdentidad();
}

Matriz4::Matriz4(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4) {
	m[0] = v1;
	m[1] = v2;
	m[2] = v3;
	m[3] = v4;
}

/**************** Definiciones de transformaciones especificas ****************/

// desplazamiento en x,y,z:
void Matriz4::setTraslacion(const float x, const float y, const float z) {
	setIdentidad();
	m[3] = Vector3(x, y, z, float(1));
}

// escalar en sx, sy, sz:
void Matriz4::setEscalar(const float sx, const float sy, const float sz) {
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
void Matriz4::setRotarX(const float theta) {
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
void Matriz4::setRotarY(const float theta) {
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
void Matriz4::setRotarZ(const float theta) {
	setIdentidad();
	m[0][0] = cos(theta);
	m[0][1] = sin(theta);
	m[1][0] = -sin(theta);
	m[1][1] = cos(theta);
}

// para cambiar un vector a una nueva base
void Matriz4::setCambioBase(const Vector3& eje1, const Vector3& eje2, const Vector3& eje3, const Vector3& origen) {
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

// Devuelve el determinante de la matriz
float Matriz4::determinante() const {
	//return determinante(0, 0, -1, TAM_MATRIZ);
	std::list<int> columnas = {0,1,2,3};
	std::list<int> filas = {0,1,2,3};
	return determinante(columnas, filas);
}

// Algoritmo general basado en: https://www.tutorialspoint.com/cplusplus-program-to-find-inverse-of-a-graph-matrix
Matriz4 Matriz4::inversa() const {
	float det = determinante();
	//std::cout << "det: " << det << std::endl;
	if (det == 0) {
		std::cerr << "Inversa imposible, det=0\n";
		return *this;
	}
	//std::cout <<"adjunta:\n"<< adj() << std::endl;
	Matriz4 inv = adj()/det;
	return inv;
}

// como string
std::string Matriz4::to_string() const {
	std::string s = "";
	for (int col = 0; col < 4; col++) {
		for (int i = 0; i < 4; i++) {
			s += std::to_string(m[col][i]) + "\t";
		}
		s += "\n";
		//s += m[i].to_string() + "\n";
	}
	return s;
}

// Devuelve la iesima fila como Vector3
Vector3 Matriz4::fila(const int i) const {
	Vector3 vFila(m[0][i], m[1][i], m[2][i], m[3][i]);
	return vFila;
}

// m[i] devuelve la iesima columna como Vector3
Vector3 Matriz4::operator [](int i) const {
	return m[i];
}

// m[i] devuelve la iesima columna como Vector3
Vector3& Matriz4::operator [](int i) {
	return m[i];
}


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
	Vector3 res(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2], v1[3]+v2[3]);
	return res;
}

// -
Vector3 operator - (const Vector3& v1, const Vector3& v2) {
	Vector3 res(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2], v1[3] - v2[3]); // TODO: revisar homogenea si eso
	return res;
}

// escalar s*v
Vector3 operator * (const float& s, const Vector3& v) {
	Vector3 res(v[0] * s, v[1] * s, v[2] * s, false);
	return res;
}


double Vector3::getMaxComponente() const {
	double max = (c[0] > c[1] && c[0]!=0) ? c[0] : c[1];
	return (c[2] > max && c[2]!=0) ? c[2] : max;
}
double Vector3::getMinComponente() const {
	double min = (c[0] < c[1]) ? c[0] : c[1];
	return (c[2] < min && c[2]!=0) ? c[2] : min;
}

// Devuelve el vector cuyo iesimo comp es v1[i]/v2[i]
Vector3 dividirComponentes(const Vector3& v1, const Vector3& v2) {
	Vector3 res;
	for (int i=0; i<3;i++) {
		if (v2[i]!=0) {
			res[i] = v1[i]/v2[i];
		}
		else {
			std::cout << "DIVISOR 0!!!!!!!!!!!\n";
			res[i] = 0;
		}
	}
	res[3] = v1[3];
	return res;
}

// escalar s/v
Vector3 operator / (const float& s, const Vector3& v) {
	Vector3 res(s/v[0], s/v[1], s/v[2], false);
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
		//return v;
	}
	Vector3 res(v[0] / s, v[1] / s, v[2] / s, v[3]/s);
	return res;
}

// escalar M/s
Matriz4 operator / (const Matriz4& m, const float& s) {
	if (s == 0) {
		std::cerr << "no dividas entre 0...Matrix" << std::endl;
		exit(1);
	}
	Matriz4 res(m[0] / s, m[1] / s, m[2] / s, m[3]/s);
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

// Prod escalar (dot product) inverso
float operator / (const Vector3& v1, const Vector3& v2) {
	float res = 0;
	for (int i = 0; i < 4; i++) {
		res += v1[i] / v2[i];
	}
	return res;
}

// cross product (producto vectorial)
Vector3 cross(const Vector3& v1, const Vector3& v2) { // TODO: comprobar
	//std::cout << "v1: " << v1 << std::endl;
	//std::cout << "v2: " << v2 << std::endl;
	Vector3 res(v1[1]*v2[2] - v1[2]*v2[1], v1[2]*v2[0] - v1[0]*v2[2], v1[0]*v2[1] - v1[1]*v2[0], false);
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

// PRE: i en [0, 3)
// Devuelve el minimo de las i-esimas coordenadas de v1 y v2
float minI(const Vector3& v1, const Vector3& v2, const int i) {
	return (v1[i]<v2[i]) ? v1[i] : v2[i];
}
// PRE: i en [0, 3)
// Devuelve el minimo de las i-esimas coordenadas de v1 y v2
float maxI(const Vector3& v1, const Vector3& v2, const int i) {
	return (v1[i]>v2[i]) ? v1[i] : v2[i];
}

// Producto M1*M2 (transformaciones, cambios de base...). Devuelve otra Matriz4
Matriz4 operator * (const Matriz4& m1, const Matriz4& m2) {
	Matriz4 res;
	for (int col = 0; col < TAM_MATRIZ; col++) {
		for (int fila = 0; fila < TAM_MATRIZ; fila++) {
			//std::cout << "calculando " << col << "," << fila << std::endl;
			res[col][fila] = m1.fila(fila) * m2[col];
			//std::cout << "fila de m1 = " << m1.fila(fila) << "\nm2.col(col) = " << m2[col] << "\nres = " << res[col][fila] << std::endl;
		}
	}
	return res;
}


// Valor absoluto de cada componente
Vector3 abs(const Vector3& v) {
	Vector3 res = v;
	for (int i = 0; i<3; i++) {
		if (res[i]<0) res[i] = -res[i];
	}
	return res;
}
