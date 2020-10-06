#include <iostream>
#include "Vector3.cpp"
//#include "Matriz4.cpp"

using namespace std;

const double PI = 3.141592653589793115997963468544185161590576171875;


// Para encapsular un poco la info de los planetas de la practica 1:
class Planeta
{
public:
	Planeta(const Vector3& _centro, const Vector3& _eje, const Vector3& _ciudad) 
		: centro(_centro), eje(_eje), ciudad(_ciudad) 
	{
		// Comprobaciones...
		//cout << (_centro.esVector() ? "joder" : "ah") << endl;
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



	// Calcula la pos y base de la estacion a partir de los parametros del planeta, 
	// y de los angulos theta (latitud desde el polo norte) y phi (longitud desde la ciudad)
	void setEstacion(float theta, float phi) { 
		Vector3 centro_ciudad = ciudad - centro;
		// Nueva base con origen en el centro del planeta, i el eje
		//cout << "vector del centro a la ciudad: " << centro_ciudad << endl;
		i = eje / eje.getModulo();
		k = cross(i, centro_ciudad);
		k = k / k.getModulo();
		j = cross(k, i);
		j = j / j.getModulo();
		//cout << "i,j,k...\n" << i << endl << j << endl << k << "\n..." << endl;
		//cout <<"Deberia cumplirse que...\n" << i << " = i = jxk =\n" << cross(j, k) << endl;

		//cout << "Deberia cumplirse que...\n" << k << " = k = ixj =\n" << cross(i, j) << endl;
		Matriz4 baseCentroPlaneta, latitud, longitud;
		baseCentroPlaneta.setCambioBase(i, j, k, centro);

		baseCentroPlaneta = baseCentroPlaneta.inversa(); // !!!!!!!!!!!!!!!!!!!!!!!1

		latitud.setRotarZ(theta);
		longitud.setRotarX(phi);


		//cout << "base en el centro: "<< baseCentroPlaneta << endl;
		estacion = centro + (eje / 2); // empieza en el polo norte
		//cout << "Polo norte: "<< estacion << endl; 
		estacion = baseCentroPlaneta * estacion; // nueva base
		//cout << "En la nueva base: " << estacion <<endl;
		estacion = latitud * estacion; // se rota a la latitud
		//cout << "En la latitud correcta: " << estacion << endl;
		estacion = longitud * estacion; // Y a la longitud
		//cout << "En la longitud correcta: " << estacion << endl;
		estacion = baseCentroPlaneta.inversa() * estacion; // Se deshace el cambio de base
		cout << "Estacion en UCS: " << estacion << endl;
		cout << "la ciudad era" << ciudad << endl;

		Matriz4 baseEstacion;
		
		kEstacion = estacion-centro;
		kEstacion = kEstacion / kEstacion.getModulo();
		jEstacion = cross(kEstacion, i);
		jEstacion = jEstacion / jEstacion.getModulo();
		//cout << "eje:" << i << endl;
		iEstacion = cross(jEstacion, kEstacion);
		iEstacion = iEstacion / iEstacion.getModulo();
		//jEstacion = cross(kEstacion, iEstacion);


		cout << "-------------------------------\n";
		cout << "ESTACION:\ni,j,k...\n" << iEstacion << endl << jEstacion << endl << kEstacion << "\n..." << endl;
		cout <<"Deberia cumplirse que...\n" << iEstacion << " = i = jxk =\n" << cross(jEstacion, kEstacion) << endl;
		cout << "Y tb Deberia cumplirse que...\n" << jEstacion << " = j = kxi =\n" << cross(kEstacion, iEstacion) << endl;
		cout << "Y tb Deberia cumplirse que...\n" << kEstacion << " = k = ixj =\n" << cross(iEstacion, jEstacion) << endl;

		cout << "-------------------------------\n";

		//estacion = ciudad; // TODO: transformaciones con angulos, etc......

		// Base:
		//k = (estacion - centro) / (estacion-centro).getModulo(); // vector del centro a la estacion entre su propio modulo, para que sea modulo 1
		// cout << "el modulo de k es " << k.getModulo() << " (deberia ser 1 creo)" << endl;
	
	}

	Vector3 getEstacion() const {
		return estacion;
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
		return i;
	}

	Vector3 getJ() const {
		return j;
	}

	Vector3 getK() const {
		return k;
	}

	Matriz4 getMatrizCambioBase() const {
		// TODO: magia
		Matriz4 cambioBase;
		cambioBase.setCambioBase(iEstacion, jEstacion, kEstacion, estacion);
		return cambioBase;
	}

private:
	Vector3 centro; // punto UCS del centro del planeta
	Vector3 eje; // vector (del polo sur al norte)
	Vector3 ciudad; // punto UCS de la ciudad

	float radio; // solo para comprobar (final punto 2 enunciado)

	// Estacion, calculada a partir del centro, el eje, la ciudad y los dos angulos....
	Vector3 estacion;

	// Base centro planeta:
	Vector3 i; 
	Vector3 j;
	Vector3 k;

	// Base estacion:
	Vector3 iEstacion;
	Vector3 jEstacion;
	Vector3 kEstacion;
};

/**************** OPERADORES ****************/

// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Planeta& p)
{
	os << p.to_string();
	return os;
}



void probarInversa() {
	Matriz4 m;
	Vector3 punto(10, 0, 0, true); // punto a 10 uds a la dcha (x)
	cout << "------------ Inversa ------------\n";
	cout << "Punto p:\n" << punto << endl;
	m.setCambioBase(Vector3(1, 0, 20, false), Vector3(3, 1, 4, false), Vector3(2, 0, 1, false), punto); // me vale cualquier matriz
	Matriz4 m1 = m.inversa();
	cout << "M=\n" << m << endl << "M^-1 =\n" << m1 << "\nsegun internete:\n(buscar en internete)" << endl;
	cout << "M*M^-1 =\n" << m*m1 << "\n(deberia ser Identidad)\n";
	/*
	Matriz4 m2(Vector3(1, 2, 3, true), Vector3(5, 5, 5, true), Vector3(9, 10, 11, true), Vector3(1, 0, 0, true)); // me vale cualquier matriz
	cout << "M=\n" << m2 << endl << "det(M) = " << m2.determinante() << " segun internete, -8" << endl;
	*/
	cout << "Traslacion: \n";
	m.setTraslacion(2,3,4); // me vale cualquier matriz
	m1 = m.inversa();
	cout << "M=\n" << m << endl << "M^-1 =\n" << m1 << "\nsegun internete:\n(buscar en internete)" << endl;
	cout << "M*M^-1 =\n" << m * m1 << "\n(deberia ser Identidad)\n";
}



void probarDeterminante() {
	Matriz4 m;
	Vector3 punto(10, 0, 0, true); // punto a 10 uds a la dcha (x)
	cout << "Punto p:\n" << punto << endl;
	m.setCambioBase(Vector3(1, 0, 20, false), Vector3(3, 1, 4, false), Vector3(2, 0, 1, false), punto); // me vale cualquier matriz
	cout << "M=\n" << m << endl << "det(M) = " << m.determinante() << " segun internete, -39" << endl;

	Matriz4 m2(Vector3(1, 2, 3, true), Vector3(5,5,5, true), Vector3(9,10,11, true), Vector3(1,0,0,true)); // me vale cualquier matriz
	cout << "M=\n" << m2 << endl << "det(M) = " << m2.determinante() << " segun internete, -8" << endl;
}

// Test de cambio de base:
// p un punto en 1,0,0
// Nueva base (con matriz m) centrada en 1,0,0
// Lo que espero ------> m*p = 0,0,0........
void probarCambioBase() {
	Matriz4 m;
	Vector3 punto(1, 0, 0, true); // punto a 10 uds a la dcha (x)
	cout << "Punto p:\n" << punto << endl;
	m.setCambioBase(Vector3(1, 0, 0, false), Vector3(0, 1, 0, false), Vector3(0, 0, 1, false), punto); // nueva base con origen en el propio punto
	cout << "Matriz de cambio de base m:\n" << m << endl;
	cout << "el punto deberia acabar en 0,0,0 ya que es el propio origen de la nueva base (?) m*p:" << endl
		<< m * punto << endl << "m^-1 * p = \n" << m.inversa() * punto << endl;
	if (m * punto != Vector3(0, 0, 0, true)) {
		cout << "Pero no es asi....." << endl;
	}
}


/**************** Programa principal ****************/
int main() {
	
	// TODO: argc y esas cosas
	std::ifstream infile("thefile.txt");
}