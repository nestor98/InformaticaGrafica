#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include <chrono> // tests

#include "escena.hpp"
#include "material.hpp"

#define hrc std::chrono::high_resolution_clock

Escena::Escena(const std::shared_ptr<Camara> _c, const int _nThreads, const Escena::TipoRender tipo)
: c(_c), renderSeleccionado(tipo)//, threads(_nThreads)
{
	threads.reserve(_nThreads);
	//std::cout << "Constructor de escena: " << c->to_string() << std::endl;
	//c = std::make_shared<Camara> _c;
}

void Escena::addFigura(const std::shared_ptr<Figura> f)
{
	figuras.push_back(f);
}

void Escena::addFiguras(const std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectFiguras) {
	figuras.insert(figuras.end(), vectFiguras->begin(), vectFiguras->end());
}


void Escena::consumirTasks(Imagen& im, const Vector3& origen) {
	//std::cout<<"Bueno"<<std::endl;
	// int cuenta = 0;
	while (true) {
		int pixel;
		{ //Lock
			// Las llaves son para que la guarda solo este entre ellas (scope):
			std::lock_guard<std::mutex> guarda(mtx); // asegura la SC
			if (tasks.empty()) { // Fin cuando no quedan tasks
				break;
			}
			pixel = tasks.back();
			tasks.pop_back();
		} // unlock
		renderPixel(im, origen, pixel);
		// cuenta++;
	}
	// std::cout<<"He dibujado: " << cuenta << " pixeles\n";
}

void Escena::initThreads(Imagen& im, const Vector3& origen) {
	for (int i=0; i<threads.capacity(); i++) {
		// std::thread t1(&Escena::consumirTasks, this, std::ref(im), std::ref(origen));
		threads.emplace_back(std::thread(&Escena::consumirTasks, this, std::ref(im), std::ref(origen)));
	}
}

void Escena::waitThreads() {
	for (int i=0; i<threads.capacity(); i++) {
		threads[i].join();
	}
}

std::string Escena::to_string() const {
	std::string s = "camara:\n" + c->to_string() + "\nfiguras:";
	if (figuras.size()>50) {
		s+= "Hay " + std::to_string(figuras.size()) +" figuras, no las listo todas\n";
	}
	else {
		for (auto f : figuras) {
			s += "\n" + f->to_string();
		}
	}
	s += "El arbol es:\n" + bvh.to_string();
	return s;
}


bool tMenor (const double t, const double min) {
	return (t<min || min==-1);
}

bool tMenor (const Figura::InterseccionData& iData, const double min) {
	return tMenor(iData.t, min);
}

void Escena::testBVHRender(const std::string f1, const std::string f2) {
	hrc::time_point t1, t2;
	std::string linea = "\n------------------------\n"; // para separarlo en cout
	std::cout << "Test de render secuencial...\nSin estructuras de aceleración...\n";
	t1 = hrc::now();
	testRenderMethod(Original, f1);
	t2 = hrc::now();
	std::chrono::duration<double> tiempoNormal = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "Tiempo de render original = " << tiempoNormal.count() << " segundos" << linea<<"Tiempo de construccion BVH...\n";
	t1 = hrc::now();
	bvh.construirArbol(figuras);
	t2 = hrc::now();
	std::chrono::duration<double> tiempoConstruccion = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

	std::cout << "Tiempo Construccion = " << tiempoConstruccion.count() << " segundos" << linea<<"Tiempo de render BVH...\n";
	t1 = hrc::now();
	testRenderMethod(Escena::BVH, f2);
	t2 = hrc::now();
	std::chrono::duration<double> tiempoBVH = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "Tiempo de render con BVH = " << tiempoBVH.count() <<" segundos"<<linea;
	auto bvhTotal = tiempoBVH.count()+tiempoConstruccion.count();
	std::cout << "BVH total: " <<  bvhTotal << linea << "bvhTotal/Original = "
					 	<< bvhTotal/tiempoNormal.count() << linea << "(Speedup, Original/bvhTotal = "
						<< tiempoNormal.count()/bvhTotal << ")" << linea;
}

// metodo tiene que ser 0 para Vector o 1 para bvh
void Escena::testRenderMethod(const Escena::Metodo metodo, const std::string fichero) const {
	// iterar para cada pixel de la camara:
		// lanzar un rayo y colorear ese pixel del color del objeto con el que intersecte
	Vector3 o = c->getPos();
	// Vector3 ultimaDir(c->getRayoPixel(0)); // DEBUG
	Imagen im(c->getPixelesY(), c->getPixelesX());
	// std::cout <<" P3\n#MAX=1\n# out/rayos.ppm\n400 400\n255";
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		// std::cout << "pixel "<<pixel<<"\n";
		if (metodo == Original) renderPixelVector(im, o, pixel);
		else renderPixel(im, o, pixel); // metodo BVH
	}
	im.guardar(fichero);
}

// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
// Itera todo el vector de figuras
void Escena::renderPixelVector(Imagen& im, const Vector3& o, const int pixel) const {
	bool interseccion = false;
	Color color(0.0,0.0,0.0);
	int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
	for (int i=0; i<nRayos; i++) { // cada rayo
		Vector3 dir(c->getRayoPixel(pixel)); // una direccion
		double tMin = -1; // distancia a la figura mas cercana
		// color de la figura mas cercana, por defecto el fondo:
		Color eFigCercana = {0.2,0.2,0.2};
		for (auto figura : figuras) {
			auto iData = figura->interseccion(o,dir);
			// double t =
			// interseccion = t>0; // intersecta
			if (iData && tMenor(*iData, tMin)) {
				tMin = iData->t;
				eFigCercana = figura->getEmision(dir);
			}
		}
		color = color + eFigCercana / double(nRayos);
		// for (int j=0; j<3; j++) { // Se suma el color de la figura mas cercana /nRayos para hacer la media
		// 	color[j]+=eFigCercana[j]/nRayos;
		// }
	}
	im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}

const Color COLOR_FONDO(0,0,0);


Color Escena::ruletaRusa(const std::shared_ptr<Figura> fig, const Vector3& dir, const Vector3& pto, const GeneradorAleatorio& rngThread, const bool primerRebote) const {
	// std::cout << "En ruleta de escena" << '\n';
	//nRebotes=nRebotes-1;
	Material mat = fig->getMaterial();
	int evento = mat.ruletaRusa(rngThread, primerRebote); // devuelve un entero entre 0 y 4 en f de las probs
	Color c;
	// std::cout << "ruleta rusa" << '\n';
	if (evento == 3){// || nRebotes == 0) { // absorcion
		// std::cout << "absorcion.." << '\n';
		return c;
	} else if (evento == 2) { // REFRACCION
		Matriz4 base = baseFromVectorYOrigen(fig->getNormal(pto), pto, dir);
		// c =mat.getCoeficiente(evento); // TODO: un poco mas majo
		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, dir);
		c = pathTrace(pto, otroPath, rngThread);
	}
	else { // se procesa el evento
		Matriz4 base = fig->getBase(pto);
		c = mat.getCoeficiente(evento); // kd
		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, dir);
		//c = mat.getCoeficiente(evento); // kd
		// std::cout << "Calculo otro path\n";// << pto << " hacia " << otroPath << '\n';
		// Color radianza = pathTrace(pto, otroPath, nRebotes-1);
		// if (!(radianza == double(0))) // TODO: esto siempre devuelve 0 :((((((((((((((
		// 	std::cout << "Radianza (resultado de pathTrace): " << radianza.to_string() << '\n';
		c = c*pathTrace(pto, otroPath, rngThread); // kd * Li
		// std::cout << "c: "<<c.to_string() << '\n';
	} // TODO: otros eventos
	// TODO: tener en cuenta que pa refl y refr, wo es -vector, no +vector!!!!!!
	return c;
}

std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccion(std::vector<std::shared_ptr<Figura>> vFigs, const Vector3& o, const Vector3& dir) {
	float t = -1;
	Vector3 pto;
	std::shared_ptr<Figura> f;
	bool intersectado = false;
	for (auto fig : vFigs) {
		auto iFig = fig->interseccion(o, dir);
		// std::cout << "Probando interseccion con " <<fig << '\n';
		if (iFig) {
			if (tMenor(iFig->t, t)) {
				//std::cout << "Interseccion con  " <<fig << '\n';
				// std::cout << "he intersectado con..." << std::endl;
				// std::cout << fig << std::endl;
				t = iFig->t;
				pto = iFig->punto;
				f = fig;
				intersectado = true;
				// std::cout << "------------------" << '\n';
//
			}
		}

	}
	if (!intersectado) return {}; // std::nullopt
	return std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>(Figura::InterseccionData{t, pto}, f);
}


Color Escena::pathTrace(const Vector3& o, const Vector3& dir, const GeneradorAleatorio& rngThread, const bool primerRebote) const {
	// std::cout << "Trazando un path" << '\n';
	Color c = COLOR_FONDO;
	double t = 1;
	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccionFigura;
	// std::cout << "Voy a intersectar" << '\n';
	if (renderSeleccionado == MaterialesSinBVH) { // Sin bvh
		interseccionFigura = interseccion(figuras, o, dir);
	}
	else { // con bvh
		// std::cout << "Cuidado con la interseccion con bvh" << '\n';
		interseccionFigura = bvh.interseccion(o, dir);
	}
	// std::cout << "Hecho" << '\n';
	// auto interseccionFigura = bvh.interseccion(o, dir); //
	//auto interseccionFigura = interseccion(figuras, o, dir);
	// std::cout << "Fin interseccion..........." << '\n';
	// if (!primerRebote) {
	// 	std::cout << "No soy el primer rebote, si intersecto te lo digo" << '\n';
	// }
	if (interseccionFigura) { // intersecta con alguna
		if (!primerRebote) {
			// std::cout << "No soy el primer rebote Y he intersectado con algo" << '\n';
		}
		// std::cout << "Intersecto con algo loco" << '\n';
		// std::cout << "dist y pto: "  << t << ", " << ptoInterseccion << '\n';
		auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
		// std::cout << "antes de emisor y blabnanjasfobnabsodlnaikjnaspian: " << fig << '\n';
		if (fig->esEmisor()) { // fin de la recursión, es un emisor
			// std::cout << "a por emision" << '\n';
			// return fig->getEmision();
			c = fig->getEmision();
			if (!primerRebote) return c*1.75; // TODO: multiplicacion bestia de la iluminacion, revisar
		}
		else {
			// std::cout << "He intersectado con un no emisor" << '\n';
			Figura::InterseccionData iData = interseccionFigura->first;
			// std::cout << "A ver el pto" << '\n';
			// t = iData.t;
			Vector3 ptoInterseccion = iData.punto;
			// std::cout << "na que no emito" << '\n';
			// std::cout << "A ver la ruleta" << '\n';

			c = ruletaRusa(fig, dir, ptoInterseccion, rngThread, primerRebote);
			// std::cout << "Vuelvo de la ruleta" << '\n';

			// if (colores == VectoresWi) { // TODO: algo asi?
			// 	// DEBUG: parece que los vectores los saca bien:
				// Matriz4 base = fig->getBase(ptoInterseccion);
				// Matriz4 base = baseFromVectorYOrigen(fig->getNormal(ptoInterseccion), ptoInterseccion, dir);
				// // std::cout << base.inversa() * dir << std::endl;
				// Material mat = fig->getMaterial();
				// Vector3 otroPath = mat.getVectorSalida(base, gen, 2, dir);
				// c.setFromNormalNoAbs(otroPath);
			// }
		}
	}
	return c;///(t*t);
}


// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void Escena::renderPixel(Imagen& im, const Vector3& o, const int pixel) const {
		bool interseccion = false;
		Color color(0.0,0.0,0.0);
		int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
		GeneradorAleatorio rngThread; // generador para el thread
		for (int i=0; i<nRayos; i++) { // cada rayo
			Vector3 dir(c->getRayoPixel(pixel)); // una direccion
			Color cPixel = pathTrace(o, dir, rngThread, true); // true para que el primero siempre rebote
			color = color + cPixel;// suma de cada path / double(nRayos);
		}
		color = color / double(nRayos); // promedio
		color.clamp(1.0); // TODO: revisar
		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}


// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void Escena::renderPixelViejo(Imagen& im, const Vector3& o, const int pixel) const {
		bool interseccion = false;
		Color color(0.0,0.0,0.0);
		int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
		for (int i=0; i<nRayos; i++) { // cada rayo
			Vector3 dir(c->getRayoPixel(pixel)); // una direccion
			// color de la figura mas cercana, por defecto el fondo:
			Color eFigCercana(0.2,0.2,0.2);
			double dist=0;
			//auto distFigura = bvh.interseccion(o, dir); // distancia
			auto interseccionFigura = bvh.interseccion(o, dir); //
			if (interseccionFigura) { // intersecta con alguna
				Figura::InterseccionData iData = interseccionFigura->first;
				double t = iData.t;
				Vector3 ptoInterseccion = iData.punto;
				auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
				if (renderSeleccionado == TipoRender::Emision) {
					eFigCercana = fig->getEmision(ptoInterseccion);//o+t*dir);// se le pasa el pto de interseccion // COLOR DE FIGURA
				} else if (renderSeleccionado == TipoRender::Normales) {
					Vector3 normal = fig->getNormal(ptoInterseccion);//o+t*dir); // Normal en el pto
					eFigCercana.setFromNormal(normal); // Color para la normal
				} else if (renderSeleccionado == TipoRender::Distancia) {
					eFigCercana.setFromDistancia(t, 1, 7); // Color para la normal
				} else if (renderSeleccionado == TipoRender::Materiales) {
					// Color
					// for (int rebote = 0;rebote<MAXREBOTES; rebote++) {
					// 	Vector3 origenRebote = ptoInterseccion;
					// 	Vector3 dirRebote = fig->getRebote(ptoInterseccion);
					// 	color+=
					// }
					// eFigCercana =
					// eFigCercana.setFromDistancia(t, 1, 7); // Color para la normal
				}
			}
			color = color + eFigCercana;// suma de cada rayo / double(nRayos);
		}
		color = color / double(nRayos); // promedio
		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}


// CODIGO viejo pa enseñar las figuras y el arbol correspondiente
// for (auto fig : figuras) {
// 	std::cout << "figura y caja: " << std::endl;
// 	std::cout << fig << std::endl;
// 	std::cout << fig->getBoundingBox()->to_string() << std::endl;
// }
// std::cout << "ESCENA\n " << this->to_string() << std::endl;
// std::cout << "----------------------------------Arbol\n" << bvh.to_string() << "\n----------------------------------\n"<< std::endl;


void Escena::render(const std::string fichero) {
	hrc::time_point t1, t2;
	t1 = hrc::now();


	// std::cout<<"a construir el arbol\n";
	bvh.construirArbol(figuras);
	std::cout<<"arbol bvh construido\n";
		//
		// std::cout << "----------------------------------Arbol\n" << bvh.to_string() << "\n----------------------------------\n"<< std::endl;

	// std::cout << bvh << std::endl;
	// iterar para cada pixel de la camara:
		// lanzar un rayo y colorear ese pixel del color del objeto con el que intersecte
	Vector3 o = c->getPos();
	// Vector3 ultimaDir(c->getRayoPixel(0)); // DEBUG
	Imagen im(c->getPixelesY(), c->getPixelesX());
	// std::cout <<" P3\n#MAX=1\n# out/rayos.ppm\n400 400\n255";
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		tasks.push_back(pixel); // encolar cada pixel
		//renderPixel(im, o, pixel);
	}
	std::cout << "Inicializando threads... " << std::flush;
	initThreads(im, o); // inicializar los threads
	std::cout << "hecho" << '\n';
	waitThreads(); // y esperar a que terminen
	// im.setMaxFloat(1); // TODO: entender esta vaina
	// im.extendedReinhard();
	im.guardar(fichero); // guardar la imagen

	t2 = hrc::now();
	std::chrono::duration<double> t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "Render realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;


}

/* Implementacion secuencial:
void Escena::renderSecuencial(const std::string fichero) const {
	// iterar para cada pixel de la camara:
		// lanzar un rayo y colorear ese pixel del color del objeto con el que intersecte
	Vector3 o = c->getPos();
	// Vector3 ultimaDir(c->getRayoPixel(0)); // DEBUG
	Imagen im(c->getPixelesY(), c->getPixelesX());
	// std::cout <<" P3\n#MAX=1\n# out/rayos.ppm\n400 400\n255";
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		renderPixel(im, o, pixel);
	}
	im.guardar(fichero);
}*/


// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Escena& e) {
	os << e.to_string();
	return os;
}
