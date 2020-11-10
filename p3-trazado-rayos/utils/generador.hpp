#include <vector>
#include <memory>



#include "figura.hpp"
#include "prisma.hpp"

// Genera figuras mas complejas a partir de figuras primitivas
class GeneradorEstructuras {
  typedef std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectorFigs;

  vectorFigs figuras;
  Vector3 pos;
  Vector3 tam;

  void setMengerSponge(const Vector3& pos, const Vector3& tam, const int iteraciones = 10);

public:
  enum Estructura {MengerSponge}; // Añadir otras estructuras aqui
private:
  Estructura tipo;
public:
  // Genera la estructura a partir del punto pos
  GeneradorEstructuras(const Estructura _estructura, const Vector3& _pos, const Vector3& _tam, const int iteraciones = 3);

  void setTipo(const Estructura estructura);

  vectorFigs getVectorFiguras() const;
};
