#include <iostream>
#include <array>
#include <string>
#include <cmath>
#include <vector>
// cmath para sqrt

#include <chrono> // tests

#include "renderer.hpp"
#include "material.hpp"

#include "luzpuntual.hpp"

#define hrc std::chrono::high_resolution_clock

const Color COLOR_FONDO(0,0,0); // TODO: mirar como ponerlo mas bonito



Renderer::Renderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo, const bool _usarBVH, const float _rangoDinamico)
: usarBVH(_usarBVH), e(_e), renderSeleccionado(tipo), rangoDinamico(_rangoDinamico)//, threads(_nThreads)
{
	threads.reserve(_nThreads+1); // +1 por la barra de progreso
}

Color contribucion(const Color& emision, const double& distLuz, const Vector3& normal, const Vector3& rayoSombra) {
	Color c = emision / (distLuz * distLuz);
	return c * (std::abs(normal * rayoSombra)); // Se devuelve su color entre la distancia al cuadrado
}

Color Renderer::shadowRay(const Vector3& pto, const Vector3& normal, const int indiceluz) const {
	Color c;
	LuzPuntual luz=	e.getLuz(indiceluz);
	Vector3 rayoSombra = luz.getPos() - pto;
	double distLuz = rayoSombra.getModulo();
	rayoSombra = normalizar(rayoSombra);
	auto interseccionFigura = e.interseccion(pto, rayoSombra);
	bool contarContribucion=false;
	if (interseccionFigura) {
		if (distLuz < interseccionFigura->first.t) {
			return contribucion(luz.getEmision(), distLuz, normal, rayoSombra);
			// c = eluz / |distLuz|^2 / |normal * rayoSombra|
		}
	} else {
		c = contribucion(luz.getEmision(), distLuz, normal, rayoSombra);
		// std::cout << "c: " << c << '\n';
		return c;
	}
	// std::cout << "L: " << c << '\n';
	return c; // no esta iluminado
}

Color Renderer::luzDirecta(const Vector3& pto, const Vector3& normal) const {
	Color L;
	int numLuces = e.getNumLuces();
	for (int i = 0; i<numLuces; i++) {
		L = L + shadowRay(pto, normal, i);
	}
	return L;
}


// Si muestrearTodas, se suman todas, si no, se muestrea una aleatoriamente
Color Renderer::muestraLuzDirecta(const Vector3& pto, const Vector3& normal,
	const GeneradorAleatorio& rng) const
{
	int numLuces = e.getNumLuces();
	if (numLuces>0) { // Se muestrea una sola luz
		
		int indiceLuz = rng.rand(0, numLuces);
		Color L = shadowRay(pto, normal, indiceLuz);
		// std::cout << "L: " << L << '\n';
		return L;
  }
	return Color();
}

/*
////////////////////////////////////////////////////////////////
float desplazamiento = bumpMap.getEmision(pto)[0] / 255.0 * bumpMap.getMaxDesplaz();
Vector3 ptoDesplazado = alejarDeNormal(pto, fig.getNormal(pto), desplazamiento);
// TODO: diferenciacion xdddddddddddddddddddd
// 			 base con nueva normal y pto
// 			 si eso quitar alejardenormal abajo
//////////////////////////////////////////////////////////////
*/
Color Renderer::ruletaRusa(const std::shared_ptr<Figura> fig, const Vector3& dir,
	const Vector3& pto, const GeneradorAleatorio& rngThread, const bool primerRebote) const
{
	Material mat = fig->getMaterial();
	Color c;

	int evento = mat.ruletaRusa(rngThread, primerRebote); // devuelve un entero entre 0 y 4 en f de las probs
	if (evento == 3) {// absorcion
		// std::cout << "absorcion.." << '\n';
		return c;
	}
	else if (evento == 2) { // REFRACCION
		// Matriz4 base = baseFromVectorYOrigen(fig->getNormal(pto), pto, dir);
		Matriz4 base = fig->getBase(pto);

		c = mat.getCoeficiente(evento); // coef de refraccion en 0..0.9
		c = c/mat.getPDF(evento, primerRebote); // kr/pdf
		// std::cout << "c: " << c.to_string() << '\n';
		float kr;
		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, dir, kr);

		Vector3 ptoCorregido = alejarDeNormal(pto, -base[2]);
		if (otroPath*base[2] > 0) {
			ptoCorregido = alejarDeNormal(pto, base[2]);
		}
		c = c  * pathTrace(ptoCorregido, otroPath, rngThread);// * kr; //
	}
	else if (evento == 1) { // ------------------------ REFLEXION
		Matriz4 base = fig->getBase(pto);
		// Vector3 normal = fig->getNormal(pto);
		Vector3 normal = base[2];

		c = mat.getCoeficiente(0); // usamos el coeficiente del difuso
		if (c == double(0)) c = mat.getCoeficiente(1);
		// Vector3 otroPath = reflejar(dir, normal);
		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, dir);
		float pdf = mat.getPDF(evento, primerRebote);

		c = c/pdf*pathTrace(alejarDeNormal(pto, normal), otroPath, rngThread); // kd * Li
	}
	else { // --------------------------- DIFUSO
		if (fig->tieneTextura()) { // con textura
			c = fig->getEmision(pto); // El coeficiente es el de la textura
		}
		else { // difuso sin textura
			c = mat.getCoeficiente(evento); // Coef difuso
		}
		// if (fig->tieneBumpMap()) {
		// 	base = fig->getBase()
		// }
		Matriz4 base = fig->getBase(pto);

		// Iluminacion directa:
		Vector3 pto = alejarDeNormal(base[3], base[2]);
		Color iDirecta = muestraLuzDirecta(pto, base[2], rngThread);

		// Iluminacion indirecta:
		Vector3 otroPath = mat.getVectorSalida(base, rngThread, evento, dir);
		float pdf = mat.getPDF(evento, primerRebote);
		// std::cout << "pdf: "<<pdf << '\n';
		// Aqui, c es kd
		// TODO: REVISAR COSENOS
		float cosangulos = std::abs(otroPath*dir);
		c = c/pdf *(iDirecta + pathTrace(pto, otroPath, rngThread)*cosangulos); // kd * Li
		//c=c/pdf *(iDirecta); //USADO PARA SOLO LUZ DIRECTA
	}
	return c;
}

// Aux de path trace, solo para tipos de renders raros basados en vectores
Vector3 vectorTipoRender(const Renderer::TipoRender tipoRender, const std::shared_ptr<Figura> fig, const Vector3& dir, const Vector3& ptoInterseccion, const GeneradorAleatorio& gen) {
	Material mat = fig->getMaterial();
	if (tipoRender == Renderer::VectoresWiRefraccion){
		// Matriz4 base = baseFromVectorYOrigen(fig->getNormal(ptoInterseccion), ptoInterseccion, dir);
			Matriz4 base = fig->getBase(ptoInterseccion);
		bool inside = false;
		if (dir * base[2] > 0) {
			base[2] = -base[2];
			inside = true;
		}
		// Vector3 otroPath =
		// std::cout << base.inversa() * dir << std::endl;
		Material mat = fig->getMaterial();
		return mat.getVectorSalida(base, gen, 2, dir);
	}
	else if (tipoRender == Renderer::VectoresWiReflexion) {
		Matriz4 base = fig->getBase(ptoInterseccion);
		return mat.getVectorSalida(base, gen, 1, dir);
	}
	else if (tipoRender == Renderer::VectoresWiDifusos) {
		Matriz4 base = fig->getBase(ptoInterseccion);
		return mat.getVectorSalida(base, gen, 0, dir);
	}
	else if (tipoRender == Renderer::Normales) {
		// return fig->getNormal(ptoInterseccion);
		return fig->getBase(ptoInterseccion)[2];
	}
	else {
		std::cerr << "ERROR: Tipo de render desconocido" << '\n';
		exit(1);
	}
}


Color Renderer::pathTrace(const Vector3& o, const Vector3& dir, const GeneradorAleatorio& rngThread, const bool primerRebote) const {
	// std::cout << "Trazando un path" << '\n';
	Color c = COLOR_FONDO;
	std::optional<std::pair<Figura::InterseccionData, std::shared_ptr<Figura>>> interseccionFigura;
	// std::cout << "Voy a intersectar" << '\n';
	interseccionFigura = e.interseccion(o, dir);

	if (interseccionFigura) { // intersecta con alguna
		if (!primerRebote) {
			// std::cout << "No soy el primer rebote Y he intersectado con algo" << '\n';
		}
		auto fig = interseccionFigura->second; // Puntero a la Figura intersectada
		// std::cout << "antes de emisor y blabnanjasfobnabsodlnaikjnaspian: " << fig << '\n';
		if (fig->esEmisor()) { // fin de la recursión, es un emisor
			// std::cout << "a por emision" << '\n';
			// return fig->getEmision();
			// if (fig->)
			c = fig->getEmision(interseccionFigura->first.punto);
			// if (!primerRebote) return c*2.0; // TODO: multiplicacion bestia de la iluminacion, revisar
		}
		else {
			Figura::InterseccionData iData = interseccionFigura->first;
			// std::cout << "A ver el pto" << '\n';
			// t = iData.t;
			Vector3 ptoInterseccion = iData.punto;
			if (renderSeleccionado == Materiales) { // Path trace normal
				// std::cout << "He intersectado con un no emisor" << '\n';
				c = ruletaRusa(fig, dir, ptoInterseccion, rngThread, primerRebote);
			}
			else if (renderSeleccionado == krFresnel) {
				double kr = fig->getMaterial().krFresnel(dir, fig->getNormal(ptoInterseccion), 1.45);
				c = Color(kr); // ya va de 0 a 1
			}
			else { // otro tipo de render:
				Vector3 vector = vectorTipoRender(renderSeleccionado, fig,  dir, ptoInterseccion,rngThread);
				c.setFromNormalVector(vector); // color del vector, cada comp en un canal rgb
			}
		}
	}
	return c;///(t*t);
}


// Renderiza el <pixel> en la imagen <im>. <o> es el origen de la camara
void Renderer::renderPixel(Imagen& im, const Vector3& o, const int pixel,
const GeneradorAleatorio& rngThread) const {
		Color color(0.0,0.0,0.0);
		auto c = e.getCamara();
		int nRayos = c->getRayosPorPixel(); // nº rayos por cada pixel
		for (int i=0; i<nRayos; i++) { // cada rayo
			Vector3 dir(c->getRayoPixel(pixel, rngThread)); // una direccion
			Color cPixel = pathTrace(o, dir, rngThread, true); // true para que el primero siempre rebote
			color = color + cPixel;// suma de cada path / double(nRayos);
		}
		color = color / double(nRayos); // promedio
		// color.clamp(1.0); // TODO: revisar
		im.setPixel(color[0], color[1], color[2], pixel); // se pone el pixel de la imagen de ese color
}





void Renderer::consumirTasks(Imagen& im, const Vector3& origen) {
	//std::cout<<"Bueno"<<std::endl;
	// int cuenta = 0;
	GeneradorAleatorio rngThread;
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
		renderPixel(im, origen, pixel,rngThread);
		// cuenta++;
	}
	// std::cout<<"He dibujado: " << cuenta << " pixeles\n";
}


// Barra de progreso adaptada de: https://stackoverflow.com/a/14539953
void dibujarBarraProgreso(double porcentaje, int longitudBarra, hrc::time_point t_ini, hrc::time_point t_ahora) {
	int nCaracteres = porcentaje*longitudBarra; // numero de veces que se repite el caracter de la barra
	std::string caracter = "█"; // caracter de completado

	// std::cout  << "[" << std::flush;
	for (int i=0; i<longitudBarra; i++) {
		if (i<nCaracteres) { // Completados
			std::cout << caracter;
		}
		else {
			std::cout << "░";
		}
	}
	double t_elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t_ahora - t_ini).count();
	double s_restantes = 10;
	if (porcentaje>0) {
		s_restantes = (1.0-porcentaje)/porcentaje * t_elapsed;
	}
	std::cout << " " << int(porcentaje*100) << '%'<< " (quedan unos " << int(s_restantes)/60 << ":"<< int(s_restantes)%60 << " min)          \r" << std::flush;
}


void Renderer::progressBar(const int nPixeles) {
	hrc::time_point t1, t2;
	t1 = hrc::now();
	int longitudBarra = 50;
	while (true) {
		int nRestantes;
		{ //Lock
			// Las llaves son para que la guarda solo este entre ellas (scope):
			std::lock_guard<std::mutex> guarda(mtx); // asegura la SC
			if (tasks.empty()) { // Fin cuando no quedan tasks
				break;
			}
			nRestantes = tasks.size();
		} // unlock
		int nTerminados = nPixeles - nRestantes;
		double porcentaje = double(nTerminados)/double(nPixeles);
		t2 = hrc::now();
		dibujarBarraProgreso(porcentaje, longitudBarra, t1, t2);
		// dibujarBarraProgreso(porcentaje, longitudBarra);
		std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Duerme 2 s
	}
	dibujarBarraProgreso(1, longitudBarra, t1, t2);
	std::cout << std::endl;
}

void Renderer::initThreads(Imagen& im, const Vector3& origen, const int nPixeles) {
	for (int i=0; i<threads.capacity()-1; i++) {
		// std::thread t1(&Escena::consumirTasks, this, std::ref(im), std::ref(origen));
		threads.emplace_back(std::thread(&Renderer::consumirTasks, this, std::ref(im), std::ref(origen)));
	}
	threads.emplace_back(std::thread(&Renderer::progressBar, this, nPixeles));
}

void Renderer::waitThreads() {
	for (int i=0; i<threads.capacity(); i++) {
		threads[i].join();
	}
}

std::string Renderer::to_string() const {
	std::string s = "No has implementado el to_string de Renderer\n";
	return s;
}


void Renderer::render(const std::string fichero) {
	hrc::time_point t1, t2;
	t1 = hrc::now();
	// std::cout<<"a construir el arbol\n";
	if (usarBVH){
		e.construirBVH();
		// std::vector<std::shared_ptr<Figura>> figs;
		// e.getFiguras(figs);
		// // std::cout << "A construir el bvh.." << '\n';
		//
		//bvh.construirArbol(figs);//figuras);
		std::cout<<"arbol bvh construido\n";
	}
	auto c = e.getCamara();
	Vector3 o = c->getPos();
	Imagen im(c->getPixelesY(), c->getPixelesX());
	for (int pixel = 0; pixel<c->getNumPixeles(); pixel++) {
		tasks.push_back(pixel); // encolar cada pixel
		//renderPixel(im, o, pixel);
	}
	std::cout << "Inicializando threads... " << std::endl;
	initThreads(im, o, c->getNumPixeles()); // inicializar los threads
	// std::cout << "hecho" << '\n';
	waitThreads(); // y esperar a que terminen
	im.setMaxFloat(rangoDinamico); // TODO: entender esta vaina
	//im.guardar("out/HDR" + fichero, true); // guardar la imagen
	im.extendedReinhard();
	im.guardar("out/" + fichero, true); // guardar la imagen

	t2 = hrc::now();
	std::chrono::duration<double> t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	// t2 -> tRender2 = t2-t1
	std::cout << "\nRender realizado en " << t.count() << " segundos (" << t.count()/60.0 << " minutos)" << std::endl;
}



// para evitar el to_string en cout
std::ostream& operator<<(std::ostream& os, const Renderer& e) {
	os << e.to_string();
	return os;
}
