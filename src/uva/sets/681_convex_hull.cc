// UVA Problem 681:
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=622

#include <iostream>

#include <algorithm>
#include <exception>
#include <vector>

#include "../geometry.cc"

using std::cin;
using std::cout;

/**
 * Extensão para a classe base geometry::Point, adicionando-se
 * a possibilidade de cálculo do ângulo formado com outro ponto
 * salvando-se o resultado também em um cache interno, de modo
 * que possa não ser calculado mais de uma vez numa operação
 * de acesso aleatório (como ordenação).
 */
struct Point : public geometry::Point {
    /**
     * Retorna angulo formado pelo vetor partindo de outro ponto.
     * Utiliza cache interno do resultado (que não checa por
     * mudanças no valor do ponto comparado). Utiliza função atan2
     * de "geometry.cc".
     */
    double angleFrom(const geometry::Point& o) const {
      if (!std::isnan(angCache_)) {
        return angCache_;
      }
      return angCache_ = atan2(o, *this);
    }

    /**
     * Reinicializa o cache interno para o ângulo calculado com a
     * função acima, de forma que possa ser utilzida novamente para
     * cálculo e utilização sucessiva do resultado do ângulo com
     */
    void resetAngleCache() const {
      angCache_ = std::nan("");
    }
private:
    mutable double angCache_{std::nan("")};
};

/**
 * Comparador utilizado para ordenar os pontos pelo ângulo formado com o
 * pivô escolhido pelo algoritmo de Graham. Utiliza o campo angCache nas
 * estruturas Point para não calcular o ângulo com o pivô mais de uma vez
 * para cada ponto. Esse campo é resetado na hora que o ponto é lido em cada
 * conjunto de entrada (ver #read_polygon).
 */
struct by_angle_with {
    const Point pivot;

    by_angle_with(const Point &pivot) : pivot(pivot) {}

    // Função comparadora propriamente dita, deve retornar se p1 < p2.
    bool operator()(const Point& p1, const Point& p2) {
      double a1 = p1.angleFrom(pivot), a2 = p2.angleFrom(pivot);
      if (a1 < a2) {
        return true;
      } else if (a1 == a2) {
        // Pontos têm o mesmo ângulo com o pivô, basta usar o comparador padrão
        // definido em Point para colocar o mais próximo do pivot primeiro.
        // Isso funciona apenas porque o pivô utilizado no algoritmo é sempre o
        // mais embaixo à esquerda, não funcionaria para pivôs genéricos.
        return p1 < p2;
      }
      return false;
    }
};

/**
 * Calcula e retorna o menor elemento no vetor recebido como argumento.
 */
template<typename Type>
Type& find_min(std::vector<Type>& vec) {
  size_t min_idx = 0;
  for (size_t i = 1; i < vec.size(); i++) {
    if (vec[i] < vec[min_idx]) {
      min_idx = i;
    }
  }
  return vec[min_idx];
}

// Funções auxiliares para ler e escrever os polígonos no formato indicado
// pelo programa, convertendo-os de/para o formato esperado pelo algoritmo
// implementado (sem repetição do primeiro ponto no final).
void read_polygon(std::vector<Point> *out_pol) {
  int size;
  cin >> size;
  out_pol->resize(size);

  for (Point& p : *out_pol) {
    cin >> p;
    p.resetAngleCache();
  }

  if (out_pol->front() != out_pol->back()) {
    throw std::runtime_error("First point must be identical to last");
  }
  out_pol->pop_back();
}

void print_polygon(const std::vector<Point>& pol) {
  cout << (pol.size() + 1) << std::endl;
  for (auto& p : pol) {
    cout << p << std::endl;
  }
  cout << pol[0] << std::endl;
}


int main() {
  size_t K;
  int separator;
  std::vector<Point> points;
  std::vector<Point> hull;

  cin >> K;
  cout << K << std::endl;
  while (K --> 0) {
    read_polygon(&points);

    // Coloca o menor ponto (baseado no comparador padrão, pelo y depois x) na
    // posição inicial e ordena o resto dos pontos a partir do ângulo com ele
    std::swap(points[0], find_min(points));
    std::sort(points.begin() + 1, points.end(), by_angle_with(points[0]));

    // Algoritmo de Graham: vai adicionando pontos ao casco convexo, sempre
    // checando se a curva formada pelo ponto a ser adicionado e os dois últimos
    // no casco é "à esquerda", removendo o último ponto do casco caso contrário
    // até que forme-se uma curva à esquerda. Quando todas as curvas forem para
    // o mesmo lado, se terá o casco convexo contendo todos os pontos.
    hull.clear();
    for (Point& newPoint : points) {
      size_t size;
      while ((size = hull.size()) >= 2) {
        if (is_left_turn(hull[size - 2], hull[size - 1], newPoint)) {
          break;
        }
        hull.pop_back();
      }
      hull.push_back(newPoint);
    }

    print_polygon(hull);
    // K será 0 no último conjunto de entradas
    if (K > 0) {
      if (!(cin >> separator) || separator != -1) {
        throw std::runtime_error("Expected -1 separator");
      }
      cout << -1 << std::endl;
    }
  }
  return 0;
}
