#include <map>
#include <string>

std::string generar(const std::map<std::string, std::string>& reglas,
  const std::string& s)
{
  nueva = "";
  for (auto c : s) { // cada caracter de s
    if (auto search = reglas.find(c); search != reglas.end()) {
      // esta sintaxis para contains es vergonzosa....
      nueva += reglas[c]; // Se sustituye por su produccion
    }
    else {
      nueva += c; // Se deja
    }
  }
  return nueva;
}

std::string lsystem(const std::map<std::string, std::string>& reglas,
  const std::string& ini, const int iteraciones)
{
  string s = ini;
  for (int i = 0; i<iteraciones; i++) {
    s = generar(reglas, s);
  }
  return s;
}

std::string ejemploLSystem(const int iteraciones) {
  std::map<std::string, std::string> reglas;
  reglas["F"]="FF+[+F-F-F]-[-F+F+F]";
  return lsystem(reglas, "F", iteraciones);
}
