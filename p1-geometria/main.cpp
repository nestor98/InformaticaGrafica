#include <iostream>
#include "Vector3.cpp"
//#include "Matriz4.cpp"

using namespace std;


// Para encapsular un poco la info de los planetas de la practica 1:
class Planeta
{
public:
	Planeta(const Vector3& _centro, const Vector3& _eje, const Vector3& _ciudad) 
		: centro(_centro), eje(_eje), ciudad(_ciudad) 
	{
		radio = eje.getModulo() / 2.0; // el eje es el diametro
		float centro_ciudad = (centro - ciudad).getModulo(); // distancia del centro a la ciudad
		float diferencia = centro_ciudad - radio; // el error, debe ser menor a 10^-6
		if (diferencia < -0.000001 || 0.0000001 < diferencia) {
			cerr << "Error: el radio es " << radio << " y la distancia del centro a la ciudad " 
				 << centro_ciudad << endl << "La diferencia, " << diferencia << " no puede ser mayor a 10^-6" << endl;
			exit(1);
		}
	}



	string to_string() const {
		string s = "Centro: " + centro.to_string();
		s += "\nEje: " + eje.to_string();
		s += "\nCiudad: " + ciudad.to_string();
	}

	// getters....
	Vector3 getCentro() const {
		return centro;
	}

	Vector3 getEje() const {
		return eje;
	}

	Vector3 getCiudad() const {
		return ciudad;
	}


	// Calcular base:
	Vector3 getI() const {

	}

	Vector3 getJ() const {

	}

	Vector3 getK() const {

	}


private:
	Vector3 centro; // punto UCS del centro del planeta
	Vector3 eje; // vector (del polo sur al norte)
	Vector3 ciudad; // punto UCS de la ciudad

	float radio; // solo para comprobar (final punto 2 enunciado)
};

/**************** OPERADORES ****************/

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Planeta& p)
{
	os << p.to_string();
	return os;
}

int main() {
	
	Vector3 v1(2, 2, 2, false);
	Matriz4 m(v1, v1, v1, v1);

	cout << 5.0 * v1 << endl;
	//cout << "hey" << endl;
	Vector3 v3 = m*v1;

	//cout << "hearsljnagsnon" << endl;
	cout << v3 << endl;

	cout << "Modulo v1(" << v1 << ") = " << v1.getModulo() << endl;
	
	return 0;
}