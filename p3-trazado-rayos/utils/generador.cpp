#include <vector>
#include <memory>

#include "generador.hpp"
#include "esfera.hpp"

#include <map>
#include <string>


// ---------------------------------------------------------------------
// L-Systems, para generar strings que luego se compilen en figuras, etc
// ---------------------------------------------------------------------

std::string iterarLSystem(const std::map<char, std::string>& reglas,
  const std::string& s)
{
  std::string nueva = "";
  for (auto c : s) { // cada caracter de s
    if (auto search = reglas.find(c); search != reglas.end()) {
      // esta sintaxis para contains es vergonzosa....
      nueva += reglas.at(c); // Se sustituye por su produccion
    }
    else {
      nueva += c; // Se deja
    }
  }
  return nueva;
}

std::string lSystem(const std::map<char, std::string>& reglas,
  const std::string& ini, const int iteraciones)
{
  std::string s = ini;
  for (int i = 0; i<iteraciones; i++) {
    s = iterarLSystem(reglas, s);
  }
  return s;
}

// fuente de las reglas: http://paulbourke.net/fractals/lsys/
std::string ejemploLSystem(const int iteraciones) {
  std::map<char, std::string> reglas;
  reglas['F']="FF+[+F-F-F]-[-F+F+F]";
  return lSystem(reglas, "F", iteraciones);
}

// ---------------------------------------------------------------------
// Funcion recursiva para una esponja de Menger con los parametros dados
// ---------------------------------------------------------------------
void GeneradorEstructuras::setMengerSponge(const Vector3& pos, const Vector3& tam, const int iteraciones) {
  // Prisma cubo(pos, tam);
  // std::cout << "hm " << iteraciones<<"\n";
  if (iteraciones <= 1) { // "hoja", se genera y añade el prisma
    Prisma cubo(pos, tam);
    // cubo.setColorFromPos(30);
    cubo.setColorFromPos(pos, this->pos, this->pos + this->tam);
    // cubo.setRandomColor();
    figuras->emplace_back(std::make_shared<Prisma>(cubo));
  }
  else { // Se generan 3 * 3 * 3 cubos (menos los que estan en el centro de alguna cara o el central)
    Vector3 tamSubCubos = tam/3.0; // tamaño de cada sub-cubo sera 1/3 del actual
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (!(i==1 && j==i)) { // si coinciden las dos coordenadas es el central y no se hace
          for (int k = 0; k<3; k++) {
            if (!( (i==1 && k==i) || (j==1 && k==j) )) { // si coincide k con cualquier otra coord y es la central, no se hace
              Vector3 desplazamiento(i*tamSubCubos[0], j*tamSubCubos[1], k*tamSubCubos[2], false);
              setMengerSponge(pos + desplazamiento, tamSubCubos, iteraciones-1);
            }
          }
        }
      }
    }
  }
}

void GeneradorEstructuras::setArbolPrismas(const Matriz4& base, const Vector3& tam, const int iteraciones)
{
  std::string lsys = ejemploLSystem(iteraciones); // La cadena con la magia
  std::cout << "Lsystem: " << lsys << '\n';
  // Variables a ir modificando:
  Matriz4 baseActual = base;
  Vector3 tamActual = tam;
  //double anchuraAltura = 0.25; // Relacion anchura/altura
  double reduccionTam = 1; // Reduccion en cada movimiento adelante

  // Rotaciones:
  double anguloDcha = gradosARad(22.5);
  Matriz4 rotacionDcha; rotacionDcha.setRotarY(anguloDcha);
  double anguloIzq = gradosARad(23);
  Matriz4 rotacionIzq; rotacionIzq.setRotarY(-anguloIzq);

  // Color c(0.9);
  double redColor = 0.9;

  std::vector<Matriz4> pilaMatrices;// Permiten volver a una base anterior con los corchetes
  for (auto mov : lsys) { // Cada movimiento
    if (mov == 'F') { // Forward, se dibuja rama
      PrismaRotable rama(baseActual, 1.1*tamActual); // 1.1 para que no se vean rendijas entre los prismas
      rama.setMaterial(DIFUSO_MARRON_OSCURO);
      figuras->emplace_back(std::make_shared<PrismaRotable>(rama));
      // Actualizamos:
      baseActual[3] = baseActual[3] + baseActual[2]*tamActual[2];
      tamActual = tamActual * reduccionTam;
      // c=c*redColor;
      // TODO: COMPROBARRRRRR:
      // Se desplaza la base
    } else if (mov == '+') {  // Rotar dcha
      // c=Color(0,0.9,0); // DEBUG: verde
      rotarAlrededorDePto(baseActual, rotacionDcha);
    } else if (mov == '-') { // Rotar izq
      // c=Color(0.9,0,0); // DEBUG: rojo
      rotarAlrededorDePto(baseActual, rotacionIzq);
    } else if (mov == '[') { // Guardar posicion
      pilaMatrices.emplace_back(baseActual);
    } else if (mov == ']') { // Volver a ultima pos guardada
      baseActual = pilaMatrices.back();
      pilaMatrices.pop_back();
    } else {
      std::cerr << "ERROR en setArbolPrismas, movimiento " << mov
        << " desconocido" << '\n';
      exit(1);
    }
  }
}




// Genera la estructura a partir del punto pos
GeneradorEstructuras::GeneradorEstructuras(const Estructura _estructura, const Vector3& _pos, const Vector3& _tam, const int iteraciones) :
pos(_pos), tam(_tam), figuras(new std::vector<std::shared_ptr<Figura>>()), tipo(_estructura)
{
  if (_estructura == GeneradorEstructuras::Estructura::MengerSponge) {
    int iteracionesCap = iteraciones;
    if (iteraciones > 5) {
      std::cout << "Mala idea hacer mas de 5 iteraciones, te lo dejo en 5\n";
      iteracionesCap = 5;
    }
    setMengerSponge(_pos, _tam, iteracionesCap);
    std::cout << "Esponja generada!\n" << "(Contiene " << figuras->size() << " cubos)\n";
  }
  else if (_estructura == GeneradorEstructuras::Estructura::ArbolPrismas) {
    Matriz4 base;
    base.setCambioBase(FRONT, LEFT, UP, _pos);
    setArbolPrismas(base, tam/iteraciones, 1);
  }
  else {
    std::cerr << "Estructura desconocida (de momento solo tenemos MengerSponge)\n";
    exit(1);
  }
}


// Genera la estructura a partir del punto pos
GeneradorEstructuras::GeneradorEstructuras(const Estructura _estructura, const Matriz4& _base, const Vector3& _tam, const int iteraciones) :
pos(_base[3]), tam(_tam), figuras(new std::vector<std::shared_ptr<Figura>>()), tipo(_estructura)
{
  if (_estructura == GeneradorEstructuras::Estructura::MengerSponge) {
    std::cout << "GeneradorEstructuras: Advertencia, en MengerSponge no se tienen en cuenta los vectores de la base!" << '\n';
    int iteracionesCap = iteraciones;
    if (iteraciones > 5) {
      std::cout << "Mala idea hacer mas de 5 iteraciones, te lo dejo en 5\n";
      iteracionesCap = 5;
    }
    setMengerSponge(_base[3], _tam, iteracionesCap);
    std::cout << "Esponja generada!\n" << "(Contiene " << figuras->size() << " cubos)\n";
  }
  else if (_estructura == GeneradorEstructuras::Estructura::ArbolPrismas) {
    Matriz4 base = _base;
    //base.setCambioBase(FRONT, LEFT, UP, _pos);
    int iteracionesCap = iteraciones;

    if (iteraciones>4) {
      std::cout << "Mala idea hacer mas de 4 iteraciones, te lo dejo en 4\n";
      iteracionesCap = 4;
    }
    setArbolPrismas(base, tam/double(iteracionesCap), iteracionesCap);
    std::cout << "Arbol raro generado!\n" << "(Contiene " << figuras->size() << " prismas)\n";
  }
  else {
    std::cerr << "Estructura desconocida (de momento solo tenemos MengerSponge)\n";
    exit(1);
  }
}

Plano GeneradorEstructuras::setPlano(const std::string imagen, Vector3 normal, const Vector3 pos, const double dist){
    Imagen i=Imagen(imagen, true);
    Textura t=Textura(i, 2.0*dist/2.0, 2.0*dist/2.0, pos);

}


GeneradorEstructuras::GeneradorEstructuras(const Estructura _estructura,
  const std::string imagen, const double& dist, const float& multIlum)
:
tipo(_estructura), figuras(new std::vector<std::shared_ptr<Figura>>())
{
  if(_estructura== GeneradorEstructuras::Estructura::SkyBox){
    //Plano delante=setPlano(imagen+"pz.ppm", -FRONT,(FRONT*dist)+(UP*dist)+(LEFT)*dist, dist);
    Imagen ipz=Imagen(imagen+"pz.ppm", true);
    Textura tpz=Textura(ipz, 2.0*dist, 2.0*dist,(FRONT*dist)+(UP*dist)+(LEFT)*dist,multIlum);
    Plano pz=Plano(-FRONT, dist);
    Matriz4 rpz;
     rpz.setRotarY(gradosARad(-90));
    tpz.rotar(rpz);
    pz.setTextura(std::make_shared<Textura>(tpz));

    figuras->emplace_back(std::make_shared<Plano>(pz));

    Imagen inz=Imagen(imagen+"nz.ppm", true);
    Textura tnz=Textura(inz, 2.0*dist, 2.0*dist, (-FRONT*dist)+(UP*dist)+(LEFT)*dist,multIlum);
    Plano nz=Plano(FRONT, dist);
    Matriz4 rnz;
    rnz.setRotarY(gradosARad(90));
    tnz.rotar(rnz);
    nz.setTextura(std::make_shared<Textura>(tnz));
    figuras->emplace_back(std::make_shared<Plano>(nz));

    Imagen ipx=Imagen(imagen+"px.ppm", true);
    Textura tpx=Textura(ipx, 2.0*dist, 2.0*dist, (-LEFT*dist)+(UP*dist)+(FRONT)*dist,multIlum);
    Plano px=Plano(LEFT, dist);
    Matriz4 rpx;
    rpx.setRotarY(gradosARad(90));
    rnz.setRotarX(gradosARad(-90));
    tpx.rotar(rpx*rnz);
    px.setTextura(std::make_shared<Textura>(tpx));
    figuras->emplace_back(std::make_shared<Plano>(px));

    Imagen inx=Imagen(imagen+"nx.ppm", true);
    Textura tnx=Textura(inx, 2.0*dist, 2.0*dist, (UP*dist)+(-FRONT*dist),multIlum);
    Plano nx=Plano(-LEFT, dist);
    Matriz4 rnx;
    rnx.setRotarY(gradosARad(-90));
    rpx.setRotarX(gradosARad(90));
    tnx.rotar(rnx*rpx);
    nx.setTextura(std::make_shared<Textura>(tnx));
    figuras->emplace_back(std::make_shared<Plano>(nx));

    Imagen ipy=Imagen(imagen+"py.ppm", true);
    Textura tpy=Textura(ipy, 2.0*dist, 2.0*dist, (-FRONT*dist)+(LEFT*dist),multIlum);
    Plano py=Plano(-UP, dist);
    Matriz4 rpy;
    rpy.setRotarX(gradosARad(90));
    rpx.setRotarY(gradosARad(-90));
    tpy.rotar(rpy*rpx);
    py.setTextura(std::make_shared<Textura>(tpy));
    figuras->emplace_back(std::make_shared<Plano>(py));


    Imagen iny=Imagen(imagen+"ny.ppm", true);
    Textura tny=Textura(iny, 2.0*dist, 2.0*dist, (-UP*dist)+(FRONT*dist)+(LEFT)*dist,multIlum);
    Plano ny=Plano(UP, dist);
    Matriz4 rny;
    rny.setRotarX(gradosARad(90));
    rpx.setRotarY(gradosARad(-90));
    tny.rotar(rny*rpx);
    ny.setTextura(std::make_shared<Textura>(tny));
    figuras->emplace_back(std::make_shared<Plano>(ny));

    std::cout<<"Generados, "<<figuras->size()<<std::endl;

  }else{
    std::cout<<"Estructura erronea\n";
    exit(1);
  }
}

// Explosion de esferas
GeneradorEstructuras::GeneradorEstructuras(const Estructura _estructura, const Vector3& _centro,
  const double& _dmin, const double& _dmax, const double& _rmin, const double& _rmax,
  const int _nEsferas, const double& _probEmision) :
  tipo(_estructura), figuras(new std::vector<std::shared_ptr<Figura>>())
{
  if (_estructura != ExplosionEsferas) {
    std::cerr << "Este constructor solo es para GeneradorEstructuras::ExplosionEsferas" << '\n';
    exit(1);
  }
  else {
    Vector3 minpos = _centro - _dmax*Vector3();
    Vector3 maxpos = _centro + _dmax*Vector3();
    GeneradorAleatorio rng;
    Color emision;
    for (size_t i = 0; i < _nEsferas; i++) {
      double desplazamiento = rng.rand(_dmin, _dmax);
      // Vector3 pos = _centro + rng.vectorNormalAleatorioBruto() * desplazamiento;
      Vector3 pos = _centro + rng.vectorNormalAleatorio() * desplazamiento;
      Esfera e(pos, rng.rand(_rmin, _rmax));
      double eps = rng.rand01();
      if (eps < _probEmision) {
        // e.setRandomColor();
        eps = rng.rand(1.0,8.0); // multiplica la emision por algo entre 1 y 8
        emision.setFromPosGrad(pos,minpos,maxpos);
        std::cout << "emision: "<< emision*eps << '\n';
        e.setColor(emision*eps);
      }
      else {
        e.setMaterial(PLASTICO_DORADO);
      }
      figuras->emplace_back(std::make_shared<Esfera>(e));
    }
  }
}



GeneradorEstructuras::vectorFigs GeneradorEstructuras::getVectorFiguras() const {
  return figuras;
}
