#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <list>
#include <vector>
// cmath para sqrt
const double PI = 3.141592653589793115997963468544185161590576171875;

const int TAM_MATRIZ = 4;

 // Distancia de seguridad para alejar ptos de intersecciones:
const double EPSILON_NORMAL = 1e-4;


class Matriz4;

class Vector3 {
	std::array<float, 4> c; // coordenadas, con coord homogenea

public:

	/**************** Constructores ****************/
	Vector3();

	Vector3(const Vector3& original);

	Vector3(const float tam); // Devuelve un vector con todas las componentes = tam

	Vector3(float x, float y, float z, float homogenea);

	Vector3(float x, float y, float z, bool punto=false);

	/**************** Metodos ****************/

	void setCoords(std::array<float, 3> _c);
	void setCoords(float x, float y, float z, float homogenea);

	// True sii es un vector (false si es punto)
	bool esVector() const;


	void setVector();
	void setPunto();

	// Representacion en string del vector
	std::string to_string() const;

	// Devuelve el modulo del vector
	// TODO: comprobar!!
	float getModulo() const;
	float getModuloSq() const;
	void setModulo(const float mod);

	// void normalizar();

	double getMaxComponente() const;
	double getMinComponente() const;

	bool esPositivo() const;

	void toKDTreePoint(std::vector<float> &pto) const;
	const std::vector<float>& toKDTreePoint() const;

	// Cambio de sentido
	Vector3 operator - () const;

	// componente (get, a = v[2])
	float operator [](int i) const;

	// componente (set, v[2] = 3)
	float& operator [](int i);

};


// Clase matriz4, para las transformaciones de vectores
class Matriz4 {
private:
	std::array<Vector3, 4> m; // formada por 4 vectores (verticales)

	// Matriz identidad, se usa en otros metodos:
	void setIdentidad();

	// Escribe los parametros en la diagonal:
	void setDiagonal(const float d0, const float d1, const float d2, const float d3);

	// auxiliar de determinante(), saca el determinante de una submatriz
	// compuesta por las columnas con indice <columnas> y filas de indice <filas>
	float determinante(const std::list<int> &columnas, const std::list<int> &filas) const;

	// devuelve la transpuesta de la matriz de cofactores (cada elto, el determinante de las otras filas y cols
	// * signo + - + -...)
	Matriz4 adj() const;

public:
	/**************** Constructores ****************/

	// Por defecto, la identidad (no hace nada)
	Matriz4();
	Matriz4(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4);
	/**************** Definiciones de transformaciones especificas ****************/

	// desplazamiento en x,y,z:
	void setTraslacion(const float x, const float y, const float z);

	// escalar en sx, sy, sz:
	void setEscalar(const float sx, const float sy, const float sz);

	// Rotar en x
	/*
		1    0    0    0
		0   cos -sin   0
		0   sin  cos   0
		0    0    0    1
	*/
	void setRotarX(const float theta);

	// Rotar en y
	/*
		cos  0   sin   0
		0    1    0    0
	   -sin  0   cos   0
		0    0    0    1
	*/
	void setRotarY(const float theta);

	// Rotar en z
	/*
		cos -sin   0    0
		sin  cos   0    0
		 0    0    1    0
		 0    0    0    1
	 */
	void setRotarZ(const float theta);


	// para cambiar un vector a una nueva base
	void setCambioBase(const Vector3& eje1, const Vector3& eje2, const Vector3& eje3, const Vector3& origen);

	// Devuelve el determinante de la matriz
	float determinante() const;

	// Algoritmo general basado en: https://www.tutorialspoint.com/cplusplus-program-to-find-inverse-of-a-graph-matrix
	Matriz4 inversa() const;

	// como string
	std::string to_string() const;

	// Devuelve la iesima fila como Vector3
	Vector3 fila(const int i) const;

	// m[i] devuelve la iesima columna como Vector3
	Vector3 operator [](int i) const;

	Vector3& operator [](int i);



};

/***** Vectores constantes de utilidad *****/

const Vector3 UP(0,0,1,false);
const Vector3 LEFT(-1,0,0,false);
const Vector3 FRONT(0,1,0,false);

// La base por defecto es dcha,front,up,origen=0
const Matriz4 BASE_UNIVERSAL(-LEFT, FRONT, UP, Vector3(0,0,0,true));


/**************** OPERADORES ****************/

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Vector3& v);

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Matriz4& m);

// Comparaciones

// TODO: ahora, un vector y un punto con las mismas componentes no son iguales,
// revisar si seria mejor que lo fueran (cambiando el i<4 por i<3)
bool operator == (const Vector3& v1, const Vector3& v2);

bool operator != (const Vector3& v1, const Vector3& v2);

bool operator == (const Matriz4& m1, const Matriz4& m2);


// +
Vector3 operator + (const Vector3& v1, const Vector3& v2);

// -
Vector3 operator - (const Vector3& v1, const Vector3& v2);

// escalar s*v
Vector3 operator * (const float& s, const Vector3& v);

// escalar s/v
Vector3 operator / (const float& s, const Vector3& v);

// Devuelve el vector cuyo iesimo comp es v1[i]/v2[i]
Vector3 dividirComponentes(const Vector3& v1, const Vector3& v2);

// escalar s*v (orden v*s)
Vector3 operator * (const Vector3& v, const float& s);

float operator / (const Vector3& v1, const Vector3& v2);

// TODO: no dividas
// escalar v/s
Vector3 operator / (const Vector3& v, const float& s);

// escalar M/s
Matriz4 operator / (const Matriz4& m, const float& s);



// Rota la matriz <base> mediante <rotacion> alrededor del pto de la base
void rotarAlrededorDePto(Matriz4& base, const Matriz4& rotacion);


// Prod escalar (dot product)
float operator * (const Vector3& v1, const Vector3& v2);

// Devuelve el vector v normalizado:
Vector3 normalizar(const Vector3& v);

// Devuelve el minimo de las i-esimas coordenadas de v1 y v2
float minI(const Vector3& v1, const Vector3& v2, const int i);

// Devuelve el max de las i-esimas coordenadas de v1 y v2
float maxI(const Vector3& v1, const Vector3& v2, const int i);

// cross product (producto vectorial)
Vector3 cross(const Vector3& v1, const Vector3& v2);

// Producto M*v (transformaciones, cambios de base...). Devuelve otro Vector3
Vector3 operator * (const Matriz4& m, const Vector3& v);


// Producto M1*M2 (transformaciones, cambios de base...). Devuelve otra Matriz4
Matriz4 operator * (const Matriz4& m1, const Matriz4& m2);

// Valor absoluto de cada componente
Vector3 abs(const Vector3& v);


// Devuelve el vector resultante de reflejar in en la normal de la base
// la normal debe ser la tercera componente
Vector3 reflejar(const Vector3& in, const Matriz4& base);


// Devuelve el vector resultante de reflejar in en la normal
Vector3 reflejar(const Vector3& in, const Vector3& n);


Matriz4 baseFromVector(const Vector3& normal, const Vector3& pto);


Matriz4 baseFromVectorYOrigen(const Vector3& normal, const Vector3& pto,  const Vector3& origen);

Vector3 alejarDeNormal(const Vector3& punto, const Vector3& normal,
	const double dist = EPSILON_NORMAL);


double gradosARad(const double grados);

double radianesAGrados(const double rad);
