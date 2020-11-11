#include <vector>
#include <memory>

#include "generador.hpp"


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
            if (!(i==1 && k==i || j==1 && k==j)) { // si coincide k con cualquier otra coord y es la central, no se hace
              Vector3 desplazamiento(i*tamSubCubos[0], j*tamSubCubos[1], k*tamSubCubos[2], false);
              setMengerSponge(pos + desplazamiento, tamSubCubos, iteraciones-1);
            }
          }
        }
      }
    }
  }
}

// Genera la estructura a partir del punto pos
GeneradorEstructuras::GeneradorEstructuras(const Estructura _estructura, const Vector3& _pos, const Vector3& _tam, const int iteraciones) :
tipo(_estructura), pos(_pos), tam(_tam), figuras(new std::vector<std::shared_ptr<Figura>>())
{
  if (_estructura != GeneradorEstructuras::Estructura::MengerSponge) {
    std::cout << "Estructura desconocida (de momento solo tenemos MengerSponge)\n";
    exit(1);
  }
  else {
    int iteracionesCap = iteraciones;
    if (iteraciones > 5) {
      std::cout << "Mala idea hacer mas de 5 iteraciones, te lo dejo en 5\n";
      iteracionesCap = 5;
    }
    setMengerSponge(_pos, _tam, iteracionesCap);
    std::cout << "Esponja generada!\n" << "(Contiene " << figuras->size() << " cubos)\n";
  }
}

GeneradorEstructuras::vectorFigs GeneradorEstructuras::getVectorFiguras() const {
  return figuras;
}
