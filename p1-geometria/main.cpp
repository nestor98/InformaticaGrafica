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

	// TODO: sacar estacion D:
	void setEstacion(float theta, float phi) {
		Vector3 centro_ciudad = ciudad - centro;
		// Nueva base con origen en el centro del planeta, i el eje
		cout << "vector del centro a la ciudad: " << centro_ciudad << endl;
		i = eje / eje.getModulo();
		k = cross(i, centro_ciudad);
		k = k / k.getModulo();
		j = cross(k, i);
		j = j / j.getModulo();
		cout << "i,j,k...\n" << i << endl << j << endl << k << "\n..." << endl;
		cout <<"Deberia cumplirse que...\n" << i << " = i = jxk =\n" << cross(j, k) << endl;

		cout << "Deberia cumplirse que...\n" << k << " = k = ixj =\n" << cross(i, j) << endl;
		Matriz4 baseCentroPlaneta, latitud, longitud;
		baseCentroPlaneta.setCambioBase(i, j, k, centro);
		latitud.setRotarZ(theta);
		longitud.setRotarX(phi);

		cout << "base en el centro: "<< baseCentroPlaneta << endl;
		// TODO: DEBUG!!
		estacion = centro + (eje / 2); // empieza en el polo norte
		cout << "Polo norte: "<< estacion << endl; 
		estacion = baseCentroPlaneta * estacion; // nueva base
		cout << "En la nueva base: " << estacion <<endl;
		// TODO: no entiendo pq en la nueva base hay una coord > 20, si el origen esta en el centro del planeta....??
		estacion = latitud * estacion; // se rota a la latitud
		cout << "En la latitud correcta: " << estacion << endl;
		estacion = longitud * estacion; // Y a la longitud
		cout << "En la longitud correcta: " << estacion << endl;
		// TODO: inversa
		//estacion = estacion * baseCentroPlaneta.inversa(); // Se deshace el cambio de base
		//cout << "En UCS de nuevo: " << estacion << endl;




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
		cambioBase.setCambioBase(i, j, k, estacion);
		return cambioBase;
	}

private:
	Vector3 centro; // punto UCS del centro del planeta
	Vector3 eje; // vector (del polo sur al norte)
	Vector3 ciudad; // punto UCS de la ciudad

	float radio; // solo para comprobar (final punto 2 enunciado)

	// Estacion, calculada a partir del centro, el eje, la ciudad y los dos angulos....
	Vector3 estacion;

	// Base:
	Vector3 i; 
	Vector3 j;
	Vector3 k;
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
	
	// ---------------  pruebecillas:
	Vector3 u1(2, 2, 2, false);
	Matriz4 m(u1, u1, u1, u1);

	cout << 5.0 * u1 << endl;
	//cout << "hey" << endl;
	Vector3 u2 = m*u1;

	//cout << "hearsljnagsnon" << endl;
	cout << u2 << endl;

	cout << "Modulo u1(" << u1 << ") = " << u1.getModulo() << endl;
	
	m.setRotarX(30);

	cout << " matriz de rotacion en X, por ej:\n" << m << endl;

	// Test de cambio de base
	Vector3 punto(10, 0, 0, true); // punto a 10 uds a la dcha (x)
	m.setCambioBase(Vector3(1, 0, 0, false), Vector3(0, 1, 0, false), Vector3(0, 0, 1, false), punto); // nueva base con origen en el propio punto
	cout << "el punto " << punto << " deberia acabar en 0,0,0 ya que es el propio origen de la nueva base:" << endl
		 << m * punto << endl;
	if (m * punto != Vector3(0, 0, 0, true)) {
		cout << "Pero no es asi....." << endl;
	}

	/*
	// ---------------  ejercicios:
	// Primer planeta:
	Vector3 centro1(10,0,0,true); // TODO: cambiar los valores a algo con sentido
	//cout << (centro1.esVector() ? "pero bueno" : "vale") << endl;
	Vector3 eje1(10, 0, 0, false);
	Vector3 ciudad1(15, 0, 0, true);
	Planeta p1(centro1, eje1, ciudad1);
	
	p1.setEstacion(10,10); // estacion a partir de los angulos

	// Segundo planeta:
	Vector3 centro2(-10, 0, 0, true); // TODO: cambiar los valores a algo con sentido
	Vector3 eje2(10, 0, 0, false);
	Vector3 ciudad2(-15, 0, 0, true);
	Planeta p2(centro2, eje2, ciudad2);

	p2.setEstacion(10, 10); // estacion a partir de los angulos

	// Vector entre las estaciones en UCS:
	Vector3 v = p2.getEstacion() - p1.getEstacion();

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
	*/

	/**************************** Version simple ******************************/
	cout << "----------------------- programa con params simples ----------------------" << endl;
	cout << "(deberia dar los vectores 0,0,distancia y 0,0,-distancia para cada planeta)" << endl;
	// Primer planeta:
	Vector3 centro1(-20, 0, 0, true); // TODO: cambiar los valores a algo con sentido
	//cout << (centro1.esVector() ? "pero bueno" : "vale") << endl;
	Vector3 eje1(0, 20, 0, false);
	Vector3 ciudad1(-10, 0, 0, true);
	Planeta p1(centro1, eje1, ciudad1);

	p1.setEstacion(10, 10); // estacion a partir de los angulos

	// Segundo planeta:
	Vector3 centro2(20, 0, 0, true); // TODO: cambiar los valores a algo con sentido
	Vector3 eje2(0, -20, 0, false);
	Vector3 ciudad2(10, 0, 0, true);
	Planeta p2(centro2, eje2, ciudad2);

	p2.setEstacion(0, 0); // estacion a partir de los angulos

	// Vector entre las estaciones en UCS:
	Vector3 v = p2.getEstacion() - p1.getEstacion();

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