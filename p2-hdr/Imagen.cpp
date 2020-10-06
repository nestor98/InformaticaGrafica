#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <list>
// cmath para sqrt
#include "Imagen.hpp"

#include <fstream>

Imagen::Imagen() {}


void strToVector(const string linea, const string sep, vector& tokens) {
	
}


Imagen::Imagen(const ifstream& fichero) {
	std::string formato;
	if (!(fichero >> formato)) {
		std::cerr <<"Error, no hay titulo\n"; 
		exit(1);
	}
	if (formato != "P3") {
		std::cerr << "Formato desconocido\n";
		exit(1);
	}
	std::string linea;
	bool finHeader = false;
	while (std::getline(fichero, linea) && !finHeader) {
		int found = linea.find("#");
		if (found != std::string::npos) { // Todo esto pinta raro
			// Tiene comentario
			if (found == 0) {
				found = linea.find("MAX=");
				if (found != std::string::npos) {
					// tiene max
					linea.erase(0, 5);
					std::cout << "se supone que esto es el MAX" << linea << std::endl;

				}
			}
			else {
				linea.erase(found, linea.end()); // TODO: arreglar
			}
		}
		else { // suponemos que no hay mas comentarios TODO: cambiar esto si eso
			std::vector<int> v;
			strToVector(linea, " ", v);
			if (v.length() == 1) {
				max_in = v[0];
				finHeader = true;
			}
			else if (v.length() == 2) {
				filas = v[0];
				cols = v[1];
				pixeles.reserve(filas*cols);
			}
		}
	}
	// valores
	int valor;
	int i = 0;
	while (fichero >> valor)
	{
		pixeles[i/3][i%3] = valor; // cada 3 cambia el primer indice, el segundo rota en 0 1 2 0 1 2...
		i++;
	}
}

Imagen::Imagen(float x, float y, float z, float homogenea) {
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = homogenea;
}

Imagen::Imagen(float x, float y, float z, bool punto) {
	c[0] = x;
	c[1] = y;
	c[2] = z;
	c[3] = punto ? 1 : 0;
}

/**************** Metodos ****************/

// True sii es un vector (false si es punto)
bool Imagen::esVector() const {
	return c[3] == 0;
}

// Representacion en string del vector
std::string Imagen::to_string() const {
	std::string s = std::to_string(c[0]);
	s += " " + std::to_string(c[1]) + " " + std::to_string(c[2]) + " " + std::to_string(c[3]);
	return s;
}

// Devuelve el modulo del vector
// TODO: comprobar!!
float Imagen::getModulo() const {
	float mod = 0;
	for (int i = 0; i < 4; i++) {
		mod += c[i] * c[i]; // cada componente al cuadrado
	}
	return sqrt(mod); // raiz de la suma de los cuads
}

// Cambio de sentido
Imagen Imagen::operator - () const {
	return Imagen(-c[0], -c[1], -c[2], -c[3]);
}

// componente (get, a = v[2])
float Imagen::operator [](int i) const {
	return c[i];
}

// componente (set, v[2] = 3) 
float& Imagen::operator [](int i) {
	return c[i];
}



// Clase matriz4, para las transformaciones de vectores

// Matriz identidad, se usa en otros metodos:
void Matriz4::setIdentidad() {
	m[0] = Imagen(1, 0, 0, float(0));
	m[1] = Imagen(0, 1, 0, float(0));
	m[2] = Imagen(0, 0, 1, float(0));
	m[3] = Imagen(0, 0, 0, float(1));
}

// Escribe los parametros en la diagonal:
void Matriz4::setDiagonal(const float d0, const float d1, const float d2, const float d3) {
	m[0][0] = d0;
	m[1][1] = d1;
	m[2][2] = d2;
	m[3][3] = d3;
}

/*  Funcion mu fea con muchos modulos
// auxiliar de determinante(), saca el determinante de una parte
float determinante(const int colIni, const int filaIni, const int ignorarCol, const int tam) const {
	if (tam >= 3) std::cout << "-------------------------\n";
	if (tam >= 2) std::cout << "determinante " << colIni << "," << filaIni << " " << tam << std::endl;
	float det = 0;
	if (tam <= 1) { // tamaño es 1, el det es el de esa posicion
		det = m[colIni][filaIni];// TODO: otras columnas!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	else { // TODO: arreglar el orden, ta mal
		int s = 1; // SIGNO!
		for (int i = 0; i<tam; i++) { // se multiplica cada elto de la primera fila por el determinante de las otras filas y cols
			det += s * m[(colIni+i)%TAM_MATRIZ][filaIni] * determinante((colIni + (i + 1)%tam) % TAM_MATRIZ, filaIni + 1, (colIni-1)%TAM_MATRIZ, tam - 1);
			s = -s; // cambio de signo
		}
	}
	if (tam >= 1) 
		std::cout << "FIN determinante " << colIni << "," << filaIni << " " << tam << " -> det = " << det<< std::endl;
	return det;
}
*/

/////////////////// Ya no se usan:
/*
// auxiliar de la de debajo, devuelve true sii l contiene elto
bool Matriz4::contiene (const std::list<int> &l, const int elto) const {
	for (auto e : l) {
		if (e == elto) return true;
	}
	return false;
}
	
// original que no vale para inversa
// auxiliar de determinante(), saca el determinante de una submatriz 
// (en la que se ignoran las columnas en ignorarCol, a partir de la fila filaIni y de tamaño tam)
float Matriz4::determinante2(const std::list<int> &ignorarCol, const int filaIni, const int tam) const {
	int colIni = 0;
	while (contiene(ignorarCol, colIni)) colIni++; // saltamos las columnas no deseadas
	//if (tam >= 3) std::cout << "-------------------------\n";
	//if (tam >= 2) std::cout << "determinante2 " << colIni << "," << filaIni << " " << tam << std::endl;
	float det = 0;
	if (tam <= 1) { // tamaño es 1, el det es el de esa posicion
		det = m[colIni][filaIni];
	}
	else { 
		int s = 1; // SIGNO
		int col = colIni;
		for (int i = 0; i<tam; i++) { // tam veces
			while (contiene(ignorarCol, col)) col++; // saltamos las columnas no deseadas
				
			auto ignorar(ignorarCol);
			ignorar.push_back(col); // en el sub determinante2, ignoramos la col actual
			float algo = s * m[col][filaIni] * determinante2(ignorar, filaIni+1, tam-1);
			det += algo;

			//std::cout << col << "," << filaIni << "=" <<  m[col][filaIni] << " *det = " << algo << std::endl;
			s = -s;
			col++; // siguiente col
		}
	}
	//if (tam >= 1) 
		//std::cout << "FIN determinante " << colIni << "," << filaIni << " " << tam << " -> det = " << det<< std::endl;
	return det;
}
*/

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

Matriz4::Matriz4(const Imagen& v1, const Imagen& v2, const Imagen& v3, const Imagen& v4) {
	m[0] = v1;
	m[1] = v2;
	m[2] = v3;
	m[3] = v4;
}

/**************** Definiciones de transformaciones especificas ****************/

// desplazamiento en x,y,z:
void Matriz4::setTraslacion(const float x, const float y, const float z) {
	setIdentidad();
	m[3] = Imagen(x, y, z, float(1));
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
void Matriz4::setCambioBase(const Imagen& eje1, const Imagen& eje2, const Imagen& eje3, const Imagen& origen) {
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
	for (int i = 0; i < 4; i++) {
		for (int col = 0; col < 4; col++) {
			s += std::to_string(m[col][i]) + "\t";
		}
		s += "\n";
		//s += m[i].to_string() + "\n";
	}
	return s;
}

// Devuelve la iesima fila como Imagen
Imagen Matriz4::fila(const int i) const {
	Imagen vFila(m[0][i], m[1][i], m[2][i], m[3][i]);
	return vFila;
}

// m[i] devuelve la iesima columna como Imagen
Imagen Matriz4::operator [](int i) const {
	return m[i];
}

// m[i] devuelve la iesima columna como Imagen
Imagen& Matriz4::operator [](int i) {
	return m[i];
}


/**************** OPERADORES ****************/

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Imagen& v)
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
bool operator == (const Imagen& v1, const Imagen& v2) {
	bool iguales = true;
	for (int i = 0; i < 4; i++) {
		iguales &= v1[i] == v2[i];
	}
	return iguales;
}

bool operator != (const Imagen& v1, const Imagen& v2) {
	return !(v1 == v2);
}


// +
Imagen operator + (const Imagen& v1, const Imagen& v2) {
	Imagen res(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2], v1[3]+v2[3]);
	return res;
}

// -
Imagen operator - (const Imagen& v1, const Imagen& v2) {
	Imagen res(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2], v1[3] - v2[3]); // TODO: revisar homogenea si eso
	return res;
}

// escalar s*v
Imagen operator * (const float& s, const Imagen& v) {
	Imagen res(v[0] * s, v[1] * s, v[2] * s, false);
	return res;
}

// escalar s*v (orden v*s)
Imagen operator * (const Imagen& v, const float& s) {
	Imagen res(v[0] * s, v[1] * s, v[2] * s, false);
	return res;
}



// escalar v/s 
Imagen operator / (const Imagen& v, const float& s) {
	if (s == 0) {
		std::cerr << "no dividas entre 0..." << std::endl;
		exit(1);
	}
	Imagen res(v[0] / s, v[1] / s, v[2] / s, v[3]/s);
	return res;
}

// escalar M/s 
Matriz4 operator / (const Matriz4& m, const float& s) {
	if (s == 0) {
		std::cerr << "no dividas entre 0..." << std::endl;
		exit(1);
	}
	Matriz4 res(m[0] / s, m[1] / s, m[2] / s, m[3]/s);
	return res;
}

// Prod escalar (dot product)
float operator * (const Imagen& v1, const Imagen& v2) {
	float res = 0;
	for (int i = 0; i < 4; i++) {
		res += v1[i] * v2[i];
	}
	return res;
}

// cross product (producto vectorial)
Imagen cross(const Imagen& v1, const Imagen& v2) { // TODO: comprobar
	//std::cout << "v1: " << v1 << std::endl;
	//std::cout << "v2: " << v2 << std::endl;
	Imagen res(v1[1]*v2[2] - v1[2]*v2[1], v1[2]*v2[0] - v1[0]*v2[2], v1[0]*v2[1] - v1[1]*v2[0], false);
	//std::cout << "v1 x v2 = " << res << std::endl;
	return res;
}

// Producto M*v (transformaciones, cambios de base...). Devuelve otro Imagen
Imagen operator * (const Matriz4& m, const Imagen& v) {
	Imagen res;
	for (int i = 0; i < 4; i++) {
		res[i] = v * m.fila(i); // prod escalar del vector y la fila
	}
	return res;
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