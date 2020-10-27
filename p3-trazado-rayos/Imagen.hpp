#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt



class Imagen {
	std::vector<std::array<double, 3>> pixeles; // en un solo vector de filas*columnas
	std::string titulo;
	int filas, cols; // filas y columnas de la matriz
	//long max_in; // valor maximo de los pixeles de entrada
	float maxFloat; // Maximo a conseguir de salida, m en el enunciado
	long c; // resolucion de color (posibles valores distintos de cada valor rgb)
public:

	Imagen();
	Imagen(const std::string nombreFichero);
	Imagen(const Imagen& i2);

	Imagen(const int _filas, const int _cols, const long _c = 255, const float _maxFloat = 1, const std::string _titulo = "");

	void setPixel(const double r, const double g, const double b, const int i);

	void setPixel(const double r, const double g, const double b, const int fila, const int col);

	std::string to_string(const int elementos = 100) const;

	void guardar(const std::string fichero, bool formatoHdr = true) const;

	/* OPERACIONES */
	void clamp();
	void equalize();
	void equalizeAndClamp(const float valor);
	// Eq hasta valor, clamp desde valor
	void gamma(const float g);
	void gammaClamp(const float g, const float valor);

	void reinhard();
	void extendedReinhard();

	float getMaxFloat() const;
};

	// para evitar el to_string en cout
	std::ostream& operator<<(std::ostream& os, const Imagen& v);
