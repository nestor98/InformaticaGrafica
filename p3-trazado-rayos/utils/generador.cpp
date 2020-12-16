#include <vector>
#include <memory>

#include "generador.hpp"

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
      PrismaRotable rama(baseActual, Vector3(tamActual));
      rama.setMaterial(DIFUSO_MARRON);
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


GeneradorEstructuras::vectorFigs GeneradorEstructuras::getVectorFiguras() const {
  return figuras;
}
