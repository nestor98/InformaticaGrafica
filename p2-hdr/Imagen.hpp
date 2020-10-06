#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt



class Imagen {
	std::vector<std::array<float, 3>> pixeles; // en un solo vector de filas*columnas
	std::string titulo;
	int filas, cols; // filas y columnas de la matriz
	int max_in; // valor maximo de los pixeles de entrada
public:

	Imagen();
	Imagen(const std::string nombreFichero);
	/**************** Constructores ****************
	/*
	Imagen(const Imagen& original);


	Imagen(float x, float y, float z, bool punto);

	/**************** Metodos ****************/
	/*
	// True sii es un vector (false si es punto)
	bool esVector() const;

	// Representacion en string del vector
	std::string to_string() const;

	// Devuelve el modulo del vector
	// TODO: comprobar!!
	float getModulo() const;

	// Cambio de sentido
	Imagen operator - () const;

	*
	// componente (get, a = v[2])
	float operator [](int i) const;

	// componente (set, v[2] = 3) 
	float& operator [](int i);*/
};

/**************** OPERADORES ****************

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Imagen& v);

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Matriz4& m);

// Comparaciones

// TODO: ahora, un vector y un punto con las mismas componentes no son iguales,
// revisar si seria mejor que lo fueran (cambiando el i<4 por i<3)
bool operator == (const Imagen& v1, const Imagen& v2);

bool operator != (const Imagen& v1, const Imagen& v2);


// +
Imagen operator + (const Imagen& v1, const Imagen& v2);

// -
Imagen operator - (const Imagen& v1, const Imagen& v2);

// escalar s*v
Imagen operator * (const float& s, const Imagen& v);

// escalar s*v (orden v*s)
Imagen operator * (const Imagen& v, const float& s);



// escalar v/s 
Imagen operator / (const Imagen& v, const float& s);

// escalar M/s 
Matriz4 operator / (const Matriz4& m, const float& s);
// Prod escalar (dot product)
float operator * (const Imagen& v1, const Imagen& v2);

// cross product (producto vectorial)
Imagen cross(const Imagen& v1, const Imagen& v2);

// Producto M*v (transformaciones, cambios de base...). Devuelve otro Imagen
Imagen operator * (const Matriz4& m, const Imagen& v);


// Producto M1*M2 (transformaciones, cambios de base...). Devuelve otra Matriz4
Matriz4 operator * (const Matriz4& m1, const Matriz4& m2);


*/