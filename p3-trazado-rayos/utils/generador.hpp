#include <vector>
#include <memory>



#include "figura.hpp"
#include "prisma.hpp"
#include "plano.hpp"
#include "textura.hpp"
#include  "Imagen.hpp"

// Genera figuras mas complejas a partir de figuras primitivas
class GeneradorEstructuras {
  typedef std::shared_ptr<std::vector<std::shared_ptr<Figura>>> vectorFigs;

  Vector3 pos;
  Vector3 tam;
  vectorFigs figuras;

  void setMengerSponge(const Vector3& pos, const Vector3& tam, const int iteraciones = 10);
  void setArbolPrismas(const Matriz4& base, const Vector3& tam, const int iteraciones = 3);

public:
  enum Estructura {MengerSponge, ArbolPrismas, SkyBox, ExplosionEsferas}; // Añadir otras estructuras aqui
private:
  Estructura tipo;
public:
  // Genera la estructura a partir del punto pos
  GeneradorEstructuras(const Estructura _estructura, const Vector3& _pos, const Vector3& _tam, const int iteraciones = 3);
  // Idem, a partir de la base _base:
  GeneradorEstructuras(const Estructura _estructura, const Matriz4& _base, const Vector3& _tam, const int iteraciones=3);

  // Cubemap
  GeneradorEstructuras(const Estructura _estructura, const std::string imagen, const double& dist=10, const float& multIlum=3.0);

  // Explosion de esferas
  GeneradorEstructuras(const Estructura _estructura, const Vector3& _centro,
    const double& _dmin, const double& _dmax, const double& _rmin, const double& _rmax,
    const int _nEsferas, const double& _probEmision=1.0);


  Plano setPlano(const std::string imagen, Vector3 normal,Vector3 pos,  const double dist);

  void setTipo(const Estructura estructura);

  vectorFigs getVectorFiguras() const;
};
