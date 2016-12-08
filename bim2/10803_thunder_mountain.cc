// UVA Problem 10.803:
// https://uva.onlinejudge.org/index.php?option=onlinejudge&page=show_problem&problem=1744

#include <iostream>
#include <iomanip>

#include <algorithm>
#include <cmath>
#include <vector>
#include <limits>

#include "geometry.cc"

using City = geometry::Point;

/**
 * Classe principal para representar (os tamanhos de) todos os
 * menores caminhos possíveis entre as diferentes cidades no
 * grafo de cidades.
 */
struct PathsTable {
    static constexpr double MAX_DIST = std::numeric_limits<double>::infinity();

    size_t _count;
    double _val[102][102];

    /**
     * Initializa os caminhos entre cidades a partir da lista de
     * cidades definidas como pontos num plano. Os caminhos definidos
     * inicialmente são apenas os caminhos diretos entre as cidades
     * que sejam menores que a distância máxima (10 por padrão).
     */
    void init(const std::vector<City>& cities, double max_dist = 10) {
      _count = cities.size();

      double max_dist_sq = max_dist * max_dist;
      for (int i = 0; i < _count; i++) {
        _val[i][i] = 0;
        for (int j = i + 1; j < _count; j++) {
          double d = dist_sq(cities[i], cities[j]);
          if (d > max_dist_sq) {
            // Usamos MAX_DIST para denotar uma conexão inexistente
            // (ou ainda desconhecida) entre dois nós.
            d = PathsTable::MAX_DIST;
          } else {
            d = sqrt(d);
          }
          _val[i][j] = _val[j][i] = d;
        }
      }
    }

    // accessors
    double* operator[](int i) {
      return &_val[i][0];
    }
    const double* operator[](int i) const {
      return &_val[i][0];
    }

    /**
     * Executa o algoritmo de Floyd-Warshall propriamente dito. Funciona
     * basicamente melhorando as estimativas para o menor caminho entre
     * dois nós, considerando um conjunto cada vez maior de nós intermediários
     * por quais esse caminho pode passar.
     * A cada iteração do loop externo (k), estamos adicionando o nó k a
     * esse conjunto de nós intermediários, e para isso atualizamos todos
     * os caminhos i <-> j com o mínimo entre o caminho já conhecido ou
     * um caminho que passe pelo novo nó sendo adicionado (únicas possibilidades
     * possíveis).
     * Dessa forma, ao final da iteração externa, se terá todos os menores
     * caminhos possíveis entre todos os nós, ou MAX_DIST caso os mesmos sejam
     * disconexos.
     */
    void minimizePaths() {
      for (int k = 0; k < _count; k++) {
        for (int i = 0; i < _count; i++) {
          for (int j = i + 1; j < _count; j++) {
            double d = std::min(_val[i][j], _val[i][k] + _val[k][j]);
            if (d != _val[i][j]) {
              _val[j][i] = _val[i][j] = d;
            }
          }
        }
      }
    }

    /**
     * Procura e retorna o caminho mais longo conhecido atualmente
     * na tabela de caminhos. Resultado mudará com a primeira chamada
     * do método minimizePaths (após cada init), que realmente fará a
     * computação de todos os caminhos possíveis.
     */
    double longestPath() {
      double max = 0;
      for (int i = 0; i < _count; i++) {
        for (int j = 0; j < _count; j++) {
          max = std::max(max, _val[i][j]);
        }
      }
      return max;
    }

    /**
     * Forma de se checar se o grafo é conexo. Resultado não estará
     * correto caso minimizePaths não tiver sido chamado ainda desde
     * o último init.
     */
    bool isConnected() {
      return longestPath() == PathsTable::MAX_DIST;
    }
};

int main() {
  using namespace std;
  size_t N, numCities;
  std::vector<City> cities(102);
  PathsTable paths;

  cin >> N;
  cout << std::fixed << std::setprecision(4);
  for (int caseNum = 1; caseNum <= N; caseNum++) {
    // Leitura de variáveis
    cin >> numCities;
    cities.resize(numCities);
    for (City& c : cities) cin >> c;

    // Passos mais alto nível do algoritmo, ver docs das funções
    paths.init(cities);
    paths.minimizePaths();
    double longest = paths.longestPath();

    // Impressão do resultado
    cout << "Case #" << caseNum << ':' << std::endl;
    if (longest == PathsTable::MAX_DIST) {
      cout << "Send Kurdy";
    } else {
      cout << longest;
    }
    cout << std::endl << std::endl;
  }
  return 0;
}
