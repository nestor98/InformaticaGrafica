#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <fstream>
// cmath para sqrt
#include "Imagen.hpp"

Imagen::Imagen() : maxFloat(10000) {}

Imagen::Imagen(const Imagen& i2)
: titulo(i2.titulo), filas(i2.filas), cols(i2.cols), maxFloat(i2.maxFloat), c(i2.c)
{
	pixeles.reserve(i2.getNumPixels());
	for (int i = 0; i < i2.getNumPixels(); i++) {
		pixeles[i] = i2.pixeles[i];
	}
}

// separa <linea>, devolviendo el float a la derecha del caracter '='
// ej: linea="#MAX=23.1"  ->  devuelve 23.1
float parseMax(const std::string linea) {
	std::istringstream is{ linea }; // stream de string para facilitar la separacion
	std::string ignorar;
	std::getline(is, ignorar, '='); // ignora hasta el =
	float max = 0;
	is >> max; // saca el valor float del maximo
	//std::cout << "ignorar: " << ignorar << "\nmax = " << max << std::endl;
	return max;
}

// constructor de la imagen a partir del fichero con nombre <nombreFichero>
Imagen::Imagen(const std::string nombreFichero, bool ldr) {
	std::ifstream fichero(nombreFichero);
	std::string formato;
	std::string s;
	/*int j = 0;
	while (fichero >> s && j++ < 15) {
		std::cout << s << std::endl;
	}*/
	if (!(fichero >> formato)) {
		std::cerr <<"Error, no hay titulo\n";
		exit(1);
	}
	if (formato != "P3") {
		std::cerr << "Formato desconocido\n";
		exit(1);
	}
	std::string linea;
	// leer cabecera:
	if(ldr){
			fichero >> cols >> filas; //todo esto pa las texturas
			fichero.ignore(); // HAY QUE LLAMAR A ESTO ANTES DEL GETLINE PORQUE NO IGNORA EL \n DEL >> ANTERIOR...............................
			fichero>>c;
			// cout<<cols<<endl<<filas<<c<<endl;
			maxFloat=1;
			int valor;
			int i = 0;
			pixeles.reserve(filas * cols);
			while (fichero >> valor)
			{
				/*if (i / 3 < 25) {
					std::cout << valor << std::endl << i  << "..." << i/3 << std::endl;
				}*/
				pixeles[i/3][i%3] = valor*maxFloat/c;// cada 3 cambia el primer indice, el segundo rota en 0 1 2 0 1 2...
				//cout<<pixeles[i/3][i%3];
				i++;

			}
	//std::cout << "i/3 = " << i/3 << "\ny lineas*cols = " << filas*cols << std::endl;
	}else{

		fichero >> linea; // #MAX=...
		maxFloat = parseMax(linea); // saca el valor del max
		fichero.ignore().ignore(); // HAY QUE LLAMAR A ESTO ANTES DEL GETLINE PORQUE NO IGNORA EL \n DEL >> ANTERIOR...............................
		std::getline(fichero, linea, '\n'); // esta es # nombre_fichero, ya lo tenemos LO HE CAMBIADO PARA LA TEXTURA
		fichero >> cols >> filas;
		pixeles.reserve(filas * cols); // Reservar el tama�o del vector
		fichero >> c;

		//std::cout << "ewhehwh\n" << formato << std::endl << linea << std::endl << filas << std::endl << cols << std::endl << max_in << std::endl;
		// valores
		int valor;
		int i = 0;
		while (fichero >> valor)
		{
			/*if (i / 3 < 25) {
				std::cout << valor << std::endl << i  << "..." << i/3 << std::endl;
			}*/
			pixeles[i/3][i%3] = valor*maxFloat/c; // cada 3 cambia el primer indice, el segundo rota en 0 1 2 0 1 2...
			//cout<<pixeles[i/3][i%3];
			i++;

		}
	//std::cout << "i/3 = " << i/3 << "\ny lineas*cols = " << filas*cols << std::endl;
	}

}


Imagen::Imagen(const int _filas, const int _cols, const long _c, const float _maxFloat, const std::string _titulo)
						: titulo(_titulo), filas(_filas), cols(_cols), maxFloat(_maxFloat), c(_c)
{
	pixeles.resize(filas * cols, {0,0,0});
}

// Post: el pixel en <fila,col> tiene el valor <r,g,b>
void Imagen::setPixel(const double r, const double g, const double b, const int fila, const int col) {
	pixeles[fila*cols+col][0] = r;
	pixeles[fila*cols+col][1] = g;
	pixeles[fila*cols+col][2] = b;
}

// Post: el iesimo pixel (izq a dcha, arriba a abajo) tiene el valor <r,g,b>
void Imagen::setPixel(const double r, const double g, const double b, const int i) {
	pixeles[i][0] = r;
	pixeles[i][1] = g;
	pixeles[i][2] = b;
}

// Post: al iesimo pixel (izq a dcha, arriba a abajo) se le suma el valor <r,g,b>
void Imagen::addToPixel(const double r, const double g, const double b, const int i) {
	pixeles[i][0] += r;
	pixeles[i][1] += g;
	pixeles[i][2] += b;
}

// Divide todos los pixeles entre d
void Imagen::dividirPixels(const double& d) {
	//equalizeAndClamp(valor);
	for (int i = 0; i < filas * cols; i++) { // cada pixel
		for (auto& v : pixeles[i]) { // cada valor rgb
			v = v/d;
		}
	}
}


// Modulo matematico, nunca negativo
int modBien(const int algo, const int mod) {
	int r = algo%mod;
	return (r>=0) ? r : -r;
}

std::array<double, 3> Imagen::getPixel(const int fila, const int col)const{
	int filamod=modBien(fila, filas);
	int colmod=modBien(col, cols);
	/*if (fila>=filas || col>=cols || fila <0 || col<0) {
		std::cout <<"indice equivocado\n" << fila<<" " << col <<std::endl << "Tengo " << filas << " " <<cols << std::endl;
		exit(1);
	}
	// return {pixeles[modBien((fila*cols+col),(filas*cols))][0], pixeles[modBien((fila*cols+col),(filas*cols))][1], pixeles[modBien((fila*cols+col),(filas*cols))][2]};
	return getPixel(fila*cols+col);*/
	return getPixel(filamod*cols+colmod);
}

std::array<double, 3> Imagen::getPixel(const int i)const{
	// if (fila>=filas || col>=cols || fila <0 || col<0) {
	// 	std::cout <<"indice equivocado\n" << fila<<" " << col <<std::endl << "Tengo " << filas << " " <<cols << std::endl;
	// 	exit(1);
	// }
	return {pixeles[i][0], pixeles[i][1], pixeles[i][2]};
	// return getPixel(fila*cols+col);
}


// TODO: Re-probar las 3 siguientes:
// si v>1, v=1
void Imagen::clamp() {
	//std::cout << "hey estoy clampeando\n";
	equalizeAndClamp(1);
}

// v=v/maxFloat
void Imagen::equalize() {
	equalizeAndClamp(maxFloat);
}

// Eq hasta valor, clamp desde valor
void Imagen::equalizeAndClamp(const float valor) {
	gammaClamp(1, valor);
}

// Eq y gamma con g
void Imagen::gamma(const float g) {
	gammaClamp(g, maxFloat);
}

// Eq y gamma hasta valor, clamp desde valor
void Imagen::gammaClamp(const float g, const float valor) {
	//equalizeAndClamp(valor);
	for (int i = 0; i < filas * cols; i++) { // cada pixel
		for (auto& v : pixeles[i]) { // cada valor rgb
			if (v < valor) {
				v = v/valor;
				v = pow(v, g);
				maxFloat = 1;
			}
			else {
				v = valor;
			}
		}
	}
}


// https://64.github.io/tonemapping/#reinhard
void Imagen::reinhard() {
	for (int i = 0; i < filas * cols; i++) { // cada pixel
		for (auto& v : pixeles[i]) { // cada valor rgb
			v = v / (1.0+v);
		}
	}
	maxFloat = 1;
}

void Imagen::extendedReinhard() {
	for (int i = 0; i < filas * cols; i++) { // cada pixel
		for (auto& v : pixeles[i]) { // cada valor rgb
			v = v * (1.0 + (v / (maxFloat * maxFloat))) / (1.0 + v);
		}
	}
	maxFloat = 1;
}



// Guarda la imagen en <fichero>
void Imagen::guardar(const std::string nombreFichero, bool formatoHdr) const {
	std::ofstream fichero(nombreFichero);
	long c_salida = c;
	if (!formatoHdr) {
		// si no es en formato hdr, en lugar de usar c usamos 255
		c_salida = 255;
	}
	if (!(fichero << "P3\n#MAX=" << maxFloat << "\n# " << nombreFichero)) { // formato, #MAX, # nombre fichero
		std::cerr << "Fichero de salida no accesible\n";
		exit(1);
	}
	fichero << std::endl << cols << " " << filas << std::endl << c_salida << std::endl; // filas cols, max_in...
	for (int i = 0; i < filas; i++) { // cada fila
		for (int j = 0; j<cols; j++) { // cada pixel de la fila
			for (auto val : pixeles[i*cols+j]) { // cada valor rgb
				fichero << int(val * c_salida / maxFloat) << " ";
			}
			fichero << "\t";
		}
		fichero << std::endl; // siguiente fila
	}
	std::cout << "fin de guardar\n";
}

std::string Imagen::to_string(const int elementos) const {
	std::string s = "Primeros " + std::to_string(elementos) + " pixeles:\n";
	//std::cout << "a to string\n";
	for (int i= 0; i < elementos && i < filas*cols; i++) {

		s += std::to_string(pixeles[i][0]) + "," + std::to_string(pixeles[i][1]) + "," + std::to_string(pixeles[i][2]) + "\t";
		//std::cout << i << std::endl;

	}
	//std::cout << "ok\n";
	return s + "\n";
}


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Imagen& i) {
	//std::cout << "hey\n";
	os << i.to_string();
	//std::cout << "ciao\n";
	return os;
}

float Imagen::getMaxFloat() const {
	return maxFloat;
}
void Imagen::setResolucion(const float cmax) {
	c = cmax;
}

void Imagen::setMaxFloat(const float max) {
	maxFloat = max;
}

int Imagen::getNumPixels() const {
	return filas*cols;
}
int Imagen::getNumPixelsX() const {
	return cols;
}

int Imagen::getNumPixelsY() const {
	return filas;
}

// Devuelve el promedio del valor absoluto de la diferencia entre cada pixel en i1 e i2
double diferencia(const Imagen& i1, const Imagen& i2) {
	int nPixeles = i1.getNumPixels();
	// std::cout << "EN DIFERENCIA " << nPixeles << '\n';
	if (nPixeles != i2.getNumPixels()) {
		std::cerr << "i1 e i2 deben tener el mismo num de pixeles" << '\n';
		exit(1);
	}
	double error = 0;
	for (size_t i = 0; i < i1.getNumPixels(); i++) { // para cada pixel
		// std::cout << "Pixel " << i << '\n';
		auto p1 = i1.getPixel(i);
		auto p2 = i2.getPixel(i);
		for (size_t j = 0; j < 3; j++) { // para cada rgb
			error += std::abs(p1[j]-p2[j]); // se suma el error
			// std::cout << "error: " << error << '\n';
		}
	}
	return error/double(nPixeles); // promedio
}
