// #include <iostream>
// #include <array>
// #include <string>
// #include <cmath>
// #include <vector>
// // cmath para sqrt

// #include <chrono> // tests

// #include "progressive-pm-renderer.hpp"
// #include "Foton.hpp"


// #include "escena.hpp"



// #define hrc std::chrono::high_resolution_clock
//
//
// //*********************************************************************
// ProgressivePMRenderer::ProgressivePMRenderer(const Escena& _e, const int _nThreads, const Renderer::TipoRender tipo,
//   const bool _usarBVH, const float _rangoDinamico, const int _maxNumFotones,
//   const int _maxFotonesGlobales, const int _maxFotonesCausticos,
//   const int _nFotonesCercanos, const bool _guardarDirectos)
// : PMRenderer(_e, _nThreads, tipo, _usarBVH, _rangoDinamico,
//   _maxNumFotones,_maxFotonesGlobales,_maxFotonesCausticos,
//   _nFotonesCercanos,_guardarDirectos),
//   nFotonesTotal(0)
// {
// 	//threads.reserve(_nThreads+1); // +1 por la barra de progreso
//   hrc::time_point t1, t2;
//   std::chrono::duration<double> t; // duracion
// 	// std::cout<<"a construir el arbol\n";
// 	if (usarBVH) {
// 	  t1 = hrc::now();
//     e.construirBVH();
//     t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - hrc::now());
// 		std::cout<<"arbol bvh construido en " << t.count() <<" segundos\n";
// 	}
//   // ---------------- PREPROCESS:
//   // std::cout << "Voy a hacer el preprocess" << '\n';
//   // t1 = hrc::now();
//   // preprocess();
// 	// t2 = hrc::now();
// 	// t = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
// 	// // t2 -> tRender2 = t2-t1
// 	// std::cout << "\nPreprocess realizado en " << t.count()
//   //           << " segundos (" << t.count()/60.0 << " minutos)"
//   //           << std::endl;
// }
//
// //*********************************************************************
// // Traza un rayo de la camara y almacena sus hits en el vector de hits
// void ProgressivePMRenderer::traceRay(const Vector3& origen, const Vector3& dir,
//   int iPixel, float radio, const GeneradorAleatorio& rng)
// {
//   Vector3 o = origen;
//   Vector3 d = dir;
//   bool fin = false;
//   bool primerRebote=true;
//   while(!fin) {
//     auto intersec = e.interseccion(o, d);
//     if (intersec) { // Ha intersectado
//       auto iData = intersec->first;
//       auto fig = intersec->second;
//       Material mat = fig->getMaterial();
//       int evento = 0;// DEBUG: mat.ruletaRusa(rng, primerRebote);
//       int eventorandom = mat.ruletaRusa(rng, primerRebote);
//       if (evento==0) {
//         ptosVisibles.emplace_back(
//           Hit(
//             iData.punto, // posicion
//             fig->getNormal(iData.punto), // normal
//             d, // direccion
//             mat.getCoeficiente(eventorandom),///mat.getPDF(evento, primerRebote), // kd
//             iPixel, // Indice del pixel (orden i-d, arriba abajo)
//             Color(), // Pixel weight (????????????)
//             radio, // Radio actual de busqueda de fotones
//             0, // num de fotones acumulados actual
//             Color() // Color actual hacia la camara (0) TODO: asegurar
//           )
//         );
//         break; // DEBUG: TODO: BORRAR!!!!!
//       }
//       if (evento==3) { // Absorcion
//         fin = true;
//       }
//       else if (evento<2) { // Nueva dir y origen (difuso o reflex):
//         Matriz4 base = fig->getBase(iData.punto);
//     		d = mat.getVectorSalida(base, rng, evento, d);
//         o = alejarDeNormal(iData.punto, base[2]);
//       }
//       else { // Nueva dir y origen (refraccion o reflex fresnel)
//         Matriz4 base = fig->getBase(iData.punto);
//     		d = mat.getVectorSalida(base, rng, evento, d);
//     		o = alejarDeNormal(iData.punto, -base[2]);
//     		if (d*base[2] > 0) { // reflexion fresnel
//     			o = alejarDeNormal(iData.punto, base[2]);
//     		}
//       }
//       primerRebote=false;
//     }
//     else { // No ha intersectado
//       fin = true;
//     }
//   }
// }
//
// //*********************************************************************
// // Lanza los rayos de la camara y los almacena en el vector de hits
// void ProgressivePMRenderer::preprocess()
// {
//   auto camara = e.getCamara();
//   auto o = camara->getPos();
//   float radio = 1; // TODO: ??????????????????
//   GeneradorAleatorio rng;
//   for (int iPixel = 0; iPixel < camara->getNumPixeles(); iPixel++) {
//     for (int i = 0; i<camara->getRayosPorPixel(); i++) {
//       traceRay(o, camara->getRayoPixel(iPixel), iPixel, radio, rng);
//     }
//   }
//   std::cout << "vect de hits tiene: " << ptosVisibles.size() << '\n';
// }
//
// //*********************************************************************
// // Lanza fotones desde las luces y almacena fotones en los kdtrees
// void ProgressivePMRenderer::tracePhotons() {
//   kdTreeGlobal.clear();
//   kdTreeCaustico.clear();
//   // Trazar fotones y almacenar en los kdtrees:
//   PMRenderer::preprocess(false);
//   nFotonesTotal+=fotonesActuales;
//   std::cout << "nFotonesTotal = " << nFotonesTotal << '\n';
// }
//
// //*********************************************************************
// // Añade la contribucion de los fotones guardados al vector de hits
// void ProgressivePMRenderer::addPhotonContribution(const float& alfa) {
//   for (auto& hit : ptosVisibles) {
//     // TODO: tiene sentido usar dos kdtrees en progressive si se usan igual??
//     int nFotCercanos = 0;
//     if (isnan(hit.color[0])) {
//       std::cerr << "138: color: " << hit.color << "\tnFotones: "
//                 << hit.nFotones << "\tnFotCercanos: " << nFotCercanos << '\n';
//       exit(1);
//     }
//     Color ilumGlobal = PMRenderer::iluminacionRadioFijo(
//       kdTreeGlobal, hit.pto, hit.normal, hit.r, nFotCercanos);
//     Color ilumCaustica = PMRenderer::iluminacionRadioFijo(
//       kdTreeCaustico, hit.pto, hit.normal, hit.r, nFotCercanos);
//     hit.color = hit.color + hit.kd * ilumGlobal + hit.kd * ilumCaustica;
//
//
//
//     // Reduccion del radio (ecuacion 9, https://www.ci.i.u-tokyo.ac.jp/~hachisuka/ppm.pdf)
//     // N = nFotones, M=nFotCercanos
//
//     double dRsq=1;
//     if (nFotCercanos>0) {
//       dRsq = double(hit.nFotones+alfa*nFotCercanos)/double(hit.nFotones+nFotCercanos);
//     }
//     if (isnan(dRsq) || isnan(hit.color[0])) {
//       std::cerr << "155: color: " << hit.color << "\tdRsq: "<< dRsq << "\tnFotones: "
//                 << hit.nFotones << "\tnFotCercanos: " << nFotCercanos
//                 << "\tkd: " << hit.kd << '\n'
//                 << "global: " << ilumGlobal << "caustica: " << ilumCaustica
//                 << '\n';
//       exit(1);
//     }
//     hit.color = hit.color*dRsq; // Correccion del flujo (Ecuacion 12)
//     if (isnan(hit.color[0])) {
//       std::cerr << "161: color: " << hit.color << "\tdRsq: "<< dRsq << "\tnFotones: "
//                 << hit.nFotones << "\tnFotCercanos: " << nFotCercanos << '\n';
//       exit(1);
//     }
//     hit.r *= sqrt(dRsq); // Ecuacion 9
//     hit.nFotones += nFotonesCercanos;
//     // std::cout << "color: " << hit.color << "\tdRsq: "<< dRsq << "\tnFotones: "
//     //           << hit.nFotones << '\n';
//   }
// }
//
// // Añade la contribución de alfa al pixel iPixel
// void ProgressivePMRenderer::addToPixel(Vector3 o, Vector3 dir, Imagen& im, int iPixel, int i, const float& alfa) {
//   bool fin = false;
//   bool primerRebote=true;
//   Color c(1);
//   while(!fin) {
//     auto intersec = e.interseccion(o, dir);
//     if (intersec) { // Ha intersectado
//       auto iData = intersec->first;
//       auto fig = intersec->second;
//       Material mat = fig->getMaterial();
//       int evento = 0;// DEBUG: mat.ruletaRusa(rng, primerRebote);
//       int eventorandom = mat.ruletaRusa(rng, primerRebote);
//       if (evento==0) {
//         Vector3 xi = iData.punto;
//         Vector3 wi = dir;
//         // path contribution is W(xi,ωi)
//         // path probability density is Pe(xi,ωi)
//         float pdf = mat.getPDF(evento, primerRebote);
//         // get current radius ri from reference r1 and Eq. 15
//         addToPixel(im, iPixel, c);
//
//         break; // DEBUG: TODO: BORRAR!!!!!
//       }
//       if (evento==3) { // Absorcion
//         fin = true;
//       }
//       else if (evento<2) { // Nueva dir y origen (difuso o reflex):
//         Matriz4 base = fig->getBase(iData.punto);
//         dir = mat.getVectorSalida(base, rng, evento, dir);
//         o = alejarDeNormal(iData.punto, base[2]);
//       }
//       else { // Nueva dir y origen (refraccion o reflex fresnel)
//         Matriz4 base = fig->getBase(iData.punto);
//         dir = mat.getVectorSalida(base, rng, evento, dir);
//         o = alejarDeNormal(iData.punto, -base[2]);
//         if (dir*base[2] > 0) { // reflexion fresnel
//           o = alejarDeNormal(iData.punto, base[2]);
//         }
//       }
//       primerRebote=false;
//     }
//     else { // No ha intersectado
//       fin = true;
//     }
//   }
// }
//
//
// //*********************************************************************
// // Se itera nVeces el proceso de tracePhotons y addPhotonContribution
// // Se guarda la imagen resultante de cada (iesima) iteracion en fichero-i.ppm
// void ProgressivePMRenderer::render(int iteraciones, float alfa, const std::string& nom, bool renderAll) {
//   auto c = e.getCamara();
//   int nPixeles = c->getNumPixeles();
//   Vector3 o = c->getPos();
//   Imagen imagen(c->getPixelesX(), c->getPixelesY());
//   for (int i = 0; i<nVeces; i++) {
//     tracePhotons();
//     for (int p = 0; p<nPixeles; p++) {
//       Vector3 dir = c->getRayoPixel(p);
//       addToPixel(o, dir, imagen, p, alfa);
//     }
//     if (renderAll) {
//       imagen.guardar("out/"+nom+"-"+std::to_string(i+1)+".ppm");
//     }
//   }
//   imagen.extendedReinhard();
//   imagen.guardar("out/"+nom+".ppm");
// }
//
//
//
// //*********************************************************************
// // Renderiza el vector de ptos visibles actual en el archivo <fichero>.ppm
// void ProgressivePMRenderer::render(const std::string& fichero) {
//   auto c = e.getCamara();
//   Imagen imagen(c->getPixelesY(), c->getPixelesX());
//   // imagen.guardar("out/cero.ppm");
//   for (auto hit : ptosVisibles) {
//     // TODO: habria que dividir el color entre el num de hits con iPixel?
//     Color c;
//     if (hit.r>0){
//       // std::cout << "Hombre" << '\n';
//       c = 4*PI*hit.color/(PI*hit.r*hit.r*nFotonesTotal);
//       if (isnan(c[0])) {
//         std::cout << "color: " << hit.color << "\tr:" << hit.r
//         << "\tc: " << c << '\n';
//       }
//     }
//     else {
//       cerr << "el radio no deberia ser 0!!\n"; exit(1);
//     }
//     imagen.addToPixel(c[0],c[1],c[2],hit.iPixel);
//   }
//   imagen.extendedReinhard();
//   imagen.guardar("out/"+fichero+".ppm");
// }
