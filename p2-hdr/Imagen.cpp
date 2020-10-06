#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Imagen.hpp"

#include <fstream>

Imagen::Imagen() {}

float parseMax(const std::string linea) {
	return 0; // TODO: DE MOMENTO
}

Imagen::Imagen(const std::string nombreFichero) {
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
	fichero >> linea; // #MAX=...
	maxFloat = parseMax(linea);
	fichero.ignore().ignore(); // HAY QUE LLAMAR A ESTO ANTES DEL GETLINE PORQUE NO IGNORA EL \n DEL >> ANTERIOR...............................
	std::getline(fichero, linea, '\n'); // esta es # nombre_fichero, ya lo tenemos
	fichero >> filas >> cols;
	pixeles.reserve(filas * cols); // Reservar el tamaño del vector
	fichero >> max_in;

	//std::cout << "ewhehwh\n" << formato << std::endl << linea << std::endl << filas << std::endl << cols << std::endl << max_in << std::endl;
	// valores
	int valor;
	int i = 0;
	while (fichero >> valor)
	{
		//std::cout << valor << std::endl << i << std::endl;
		pixeles[i/3][i%3] = valor; // cada 3 cambia el primer indice, el segundo rota en 0 1 2 0 1 2...
		//std::cout << pixeles[i/3][i%3] << std::endl;
		i++;
		
	}
	//std::cout << "i/3 = " << i/3 << "\ny lineas*cols = " << filas*cols << std::endl;

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



/*
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
	}*/