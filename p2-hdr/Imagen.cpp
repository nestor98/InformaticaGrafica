#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt
#include "Imagen.hpp"

#include <fstream>

Imagen::Imagen() {}


void strToVector(const std::string linea, const std::string sep, std::vector<int>& tokens) {
	return;
}


Imagen::Imagen(const std::string nombreFichero) {
	std::ifstream fichero(nombreFichero);
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
