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
		// Comprobaciones...
		if (centro.esVector()) {
			cerr << "El centro debe ser un punto, no un vector" << endl;
			exit(1);
		}
		if (ciudad.esVector()) {
			cerr << "La ciudad debe ser un punto, no un vector" << endl;
			exit(1);
		}
		if (!eje.esVector()) {
			cerr << "El eje debe ser un vector, no un punto" << endl;
			exit(1);
		}
		// radio...
		radio = eje.getModulo() / 2.0; // el eje es el diametro
		float centro_ciudad = (centro - ciudad).getModulo(); // distancia del centro a la ciudad
		float diferencia = centro_ciudad - radio; // el error, debe ser menor a 10^-6
		if (diferencia < -0.000001 || 0.0000001 < diferencia) {
			cerr << "Error: el radio es " << radio << " y la distancia del centro a la ciudad " 
				 << centro_ciudad << endl << "La diferencia, " << diferencia << " no puede ser mayor a 10^-6" << endl;
			exit(1);
		}
	}


	// Métodos
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

	// TODO: todo lo que sigue
	// Calcular base:
	Vector3 getI() const {

	}

	Vector3 getJ() const {

	}

	Vector3 getK() const {

	}

	Matriz4 getMatrizCambioBase() const {
		// TODO: magia
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



/**************** Programa principal ****************/
int main() {
	
	// pruebecillas:
	Vector3 u1(2, 2, 2, false);
	Matriz4 m(u1, u1, u1, u1);

	cout << 5.0 * u1 << endl;
	//cout << "hey" << endl;
	Vector3 u2 = m*u1;

	//cout << "hearsljnagsnon" << endl;
	cout << u2 << endl;

	cout << "Modulo u1(" << u1 << ") = " << u1.getModulo() << endl;
	

	// ejercicios:
	// Primer planeta:
	Vector3 centro1(10,0,0,true); // TODO: cambiar los valores a algo con sentido
	Vector3 eje1(10, 0, 0, false);
	Vector3 ciudad1(10, 0, 0, true);
	Planeta p1(centro1, eje1, ciudad1);

	// Segundo planeta:
	Vector3 centro2(-10, 0, 0, true); // TODO: cambiar los valores a algo con sentido
	Vector3 eje2(10, 0, 0, false);
	Vector3 ciudad2(10, 0, 0, true);
	Planeta p2(centro2, eje2, ciudad2);

	// Vector entre las ciudades en UCS:
	Vector3 v = p2.getCiudad() - p1.getCiudad();

	// Vector en coordenadas de p1:
	Matriz4 M1 = p1.getMatrizCambioBase();
	Vector3 v1 = M1 * v;
	if (v1[2] < 0) { // si la tercera componente es menor que 0, atraviesa el planeta
		cout << "Va a atravesar el planeta 1!" << endl;
	}

	// Vector en coordenadas de p1:
	Matriz4 M2 = p2.getMatrizCambioBase();
	Vector3 v2 = -(M2 * v); // - para cambiarle el sentido
	if (v2[2] < 0) { // si la tercera componente es menor que 0, atraviesa el planeta
		cout << "Va a atravesar el planeta 2!" << endl;
	}

	return 0;
}