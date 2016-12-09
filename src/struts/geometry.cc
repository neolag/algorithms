#include <iostream>
#include <cmath>

namespace geometry {

struct Point {
    long long x, y;

    /**
     * Comparador padrão para os pontos, compara o y
     * e em seguida o x.
     */
    bool operator<(const Point& o) const {
      if (y < o.y) {
        return true;
      } else if (y == o.y && x < o.x) {
        return true;
      }
      return false;
    }

    bool operator==(const Point& o) const {
      return x == o.x && y == o.y;
    }
    bool operator!=(const Point& o) const {
      return !(*this == o);
    }
};

// Funções auxiliares para ler e escrever um Point como a maioria
// dos problemas os requerem.
std::istream& operator>>(std::istream& s, Point& p) {
  return s >> p.x >> p.y;
}
std::ostream& operator<<(std::ostream& s, const Point& p) {
  return s << p.x << " " <<  p.y;
}


/**
* Determina se a curva feita na ordem p1 -> p2 -> p3 é à esquerda.
* Isso é feito a partir do produto vetorial entre os vetores representados
* por p1 -> p2 e p2 -> p3, extendendo-se suas coordenadas Z como se fossem
* iguais a zero, e utilizando apenas a coordenada Z do resultado do produto
* vetorial para determinar o lado da "curva", onde pela regra da mão direita,
* Z > 0 => à esquerda.
*/
bool is_left_turn(const Point& p1, const Point& p2, const Point& p3) {
  long long v12x = p2.x - p1.x, v12y = p2.y - p1.y;
  long long v23x = p3.x - p2.x, v23y = p3.y - p2.y;
  return v12x * v23y - v12y * v23x > 0;
}

/**
* Calcula a fase do vetor formado por (p2 - p1).
*/
double atan2(const Point& p1, const Point& p2) {
  return std::atan2(p2.y - p1.y, p2.x - p1.x);
}

/**
 * Calcula o módulo ao quadrado do vetor formado por (p2 - p1).
 */
double dist_sq(const Point& p1, const Point& p2) {
  int dx = p1.x - p2.x, dy = p1.y - p2.y;
  return dx * dx + dy * dy;
}

}; // namespace geometry
