// UVA Problem 10.078:
// https://uva.onlinejudge.org/index.php?option=onlinejudge&page=show_problem&problem=1019

#include <iostream>

#include <vector>
#include <math.h>

#include "struts/geometry.cc"

using namespace geometry;

/**
 * Classe auxiliar para criar um array de tamanho fixo e acessá-lo
 * de forma circular.
 */
template<typename Type>
class CircularArray {
public:
    CircularArray(size_t size) : _elms(size) {}

    Type& operator[](int idx) {
      idx %= _elms.size();
      if (idx < 0) idx += _elms.size();
      return _elms[idx];
    }
private:
    std::vector<Type> _elms;
};


int main() {
  using namespace std;
  int N;
  Point first[2];
  CircularArray<Point> points(3);

  while (cin >> N && N > 0) {
    // Lê os 3 pontos iniciais a princípio, para já setar qual será
    // a direção que será construída o polígono. "Para a esquerda"
    // no caso significa que todas as curvas são à esquerda, ou seja
    // o polígono é construído na ordem anti-horaria (com a origem no
    // ponto inferior esquerdo).
    cin >> points[0] >> points[1] >> points[2];
    bool buildToLeft = is_left_turn(points[0], points[1], points[2]);
    // Salva os 2 pontos iniciais que serão necessários no final.
    first[0] = points[0];
    first[1] = points[1];

    // O algoritmo utilizado é simples, checando-se se a direção da curva
    // formada pelos 3 últimos pontos lidos mantém-se fixa por todo o
    // polígono. O único caso especial é para o último ponto, que deve ser
    // adicionalmente checado também com o primeiro e segundo pontos lidos,
    // caso a concavidade do polígono esteja neles. Para isso fazemos o loop
    // até N+2 e lidamos com o índice de forma especial quando >= N;
    bool isConvex = true;
    for (int i = 3; i < N + 2; i++) {
      if (i < N) {
        cin >> points[i];
      } else {
        points[i] = first[i - N];
      }
      if (buildToLeft != is_left_turn(points[i - 2], points[i - 1], points[i])) {
        isConvex = false;
      }
    }

    cout << (isConvex ? "No" : "Yes") << std::endl;
  }
  return 0;
}
