#include <vector>
#include <memory>



#include "figura.hpp"
#include "prisma.hpp"

// Genera figuras mas complejas a partir de figuras primitivas
class GeneradorEstructuras {
  typedef std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectorFigs;

  Vector3 pos;
  Vector3 tam;
  vectorFigs figuras;

  void setMengerSponge(const Vector3& pos, const Vector3& tam, const int iteraciones = 10);
  void setArbolPrismas(const Matriz4& base, const Vector3& tam, const int iteraciones = 3);

public:
  enum Estructura {MengerSponge, ArbolPrismas}; // Añadir otras estructuras aqui
private:
  Estructura tipo;
public:
  // Genera la estructura a partir del punto pos
  GeneradorEstructuras(const Estructura _estructura, const Vector3& _pos, const Vector3& _tam, const int iteraciones = 3);
  // Idem, a partir de la base _base:
  GeneradorEstructuras(const Estructura _estructura, const Matriz4& _base, const Vector3& _tam, const int iteraciones=3);

  void setTipo(const Estructura estructura);

  vectorFigs getVectorFiguras() const;
};
