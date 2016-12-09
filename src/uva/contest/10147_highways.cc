// UVA Problem 10.174:
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=1088

#include <iostream>
#include <fstream>

#include <algorithm>
#include <vector>

#include "struts/groups.cc"
#include "struts/heap.cc"

//#define DEBUG

#ifdef DEBUG
std::fstream cin("../in/10147.in", std::ios_base::in);
#else
using std::cin;
#endif
using std::cout;
using std::vector;

int sq(int n) {
  return n * n;
}


/**
 * Estrutura utilizada para representar uma cidade. Possui
 * um ID que será usado como o número da cidade de 1 a N.
 */
struct City {
  const int id;
  int x, y;
  mutable Group group;

  City() : id(0) {}
  City(int id) : id(id) {}
};

// Função auxiliar para ler uma cidade de stdin
std::istream& operator>>(std::istream& s, City& c) {
  return s >> c.x >> c.y;
}

/**
 * Estrutura utilizada para representar uma estrada. Guarda
 * meramente dois ponteiros para as cidades interligadas e
 * já calcula o comprimento da conexão na construção.
 */
struct Highway {
  Highway() = default;
  Highway(const City* n1, const City* n2)
          : city1(n1),
            city2(n2),
            length_sq(sq(city1->x - city2->x) + sq(city1->y - city2->y)) {}

  const City* city1;
  const City* city2;
  const int length_sq = 0;

  // Comparador padrão, o faz pelo comprimento da estrada.
  inline bool operator<(const Highway& o) const {
    return length_sq < o.length_sq;
  }
};

// Impressão de estradas na saída padrão
std::ostream& operator<<(std::ostream& s, const Highway& h) {
  return s << h.city1->id << ' ' << h.city2->id;
}

/**
 * Inicializa os grupos das cidades a partir das estradas que já existem.
 * Basicamente se faz a união de todos as duplas de cidades que contêm
 * estradas interligantes. Operação O(M), onde M é o número de estradas
 * existentes.
 */
size_t init_cities_groups(const vector<Highway> &existing) {
  size_t connectedHighways = 0;
  for (auto& h : existing) {
    if (Group::merge(h.city1->group, h.city2->group)) {
      connectedHighways++;
    }
  }
  return connectedHighways;
}

/**
 * Função auxiliar utilizada abaixo para comparar uma coleção de ponteiros
 * para uma estrutura que se deseja realmente utilizar para comparação.
 */
template<typename ptr_t>
bool by_deref(ptr_t v1, ptr_t v2) {
  return *v1 < *v2;
}

template<typename ptr_t>
bool by_deref_rev(ptr_t v1, ptr_t v2) {
  return *v2 < *v1;
}

// Utilizam-se vetores estáticos de forma a evitar alocação e dealocação
// na memória para todo caso de teste. Utiliza-se o tamanho inicial dos
// mesmos para o pior caso possível pela descrição, mas ainda deixa
// possibilidade de teste com casos ainda maiores.
static vector<Highway> buffer(750 * 751);

/**
 * Gera a lista de todas as estradas possíveis conectando as cidades
 * que ainda não estão no mesmo grupo, ordenadas pelas suas distâncias,
 * de forma a poder ser utilizada imediatamente pelo algoritmo de
 * Kruskal para árvore geradora mínima. Operação O(N^2 log N), devido à
 * ordenação de uma lista de N elementos (N=nº cidades).
 */
const vector<Highway*>& gen_sorted_candidates(const vector<City>& cities) {
  // Como serão gerados vários objetos da classe Highway que terão de ser
  // ordenados pelos comprimentos das estradas que representam, utilizamos
  // uma lista de ponteiros para os mesmos de forma a evitar muitas operações
  // de memória. Assim temos um buffer de memória onde os objetos serão
  // devidamente alocados, e um vetor de ponteiros que será realmente utilizado.
  static vector<Highway*> candidates(buffer.capacity());
  candidates.clear();
  buffer.clear();

  // Constrói-se a estrada entre cada uma das combinações de 2 cidades
  // possíveis que ainda não estejam conectadas.
  for (int j = 1; j < cities.size(); j++) {
    for (int i = 0; i < j; i++) {
      if (&cities[i].group != &cities[j].group) {
        // Como utilizamos o buffer de memória, são duas operações para
        // criação da estrada.
        buffer.emplace_back(&cities[i], &cities[j]);
        candidates.emplace_back(&buffer.back());
      }
    }
  }
  std::sort(candidates.begin(), candidates.end(), by_deref<Highway*>);
  return candidates;
}

/**
 * Dada uma lista de candidatos de estradas diferentes a serem adicionados,
 * faz-se a escolha de 'count' deles apenas se forem realmente necessários.
 * Serão necessários se, no momento de escolha, interliguem cidades que ainda
 * estão em grupos diferentes. Quando escolhidas, as cidades interligadas têm
 * também seus grupos unidos, de forma que não sejam escolhidas estradas
 * desnecessárias para a árvore geradora mínima desejada. Operação O(N^2),
 * de iteração por uma lista de no máximo N^2-1 elementos (N=nº cidades).
 */
const vector<Highway*>& pick_highways(const vector<Highway*>& sorted_candidates,
                                      size_t city_count) {
  // Estrutura estática para retorno seguindo padrão anterior.
  static vector<Highway*> created(751);
  created.clear();

  for (Highway* high : sorted_candidates) {
    if (Group::merge(high->city1->group, high->city2->group)) {
      created.push_back(high);
      if (high->city1->group.size() == city_count) break;
    }
  }
  return created;
}

/**
 *  Implementação mais alto nível do algoritmo de Kruskal para esse problema.
 *  Após ler-se as cidades e estradas existentes, incializamos as cidades
 *  que já estejam conectadas como num mesmo grupo. Em seguida geramos todas
 *  as estradas que conectam as cidades que ainda não estão conectadas, e
 *  ordenamos essa lista pelo comprimento das mesmas. Finalmente, seguindo
 *  essa ordem de escolha das estradas faltantes, escolhem-se N-M-1 estradas
 *  a serem construídas, dado que no momento de escolha ainda estejam em
 *  grupos diferentes. N e M usados acima referem-se ao número de cidades e
 *  número de estradas pre-existentes respectivamente. Algoritmo total de
 *  complexidade O(N^2 log N + M), devido as complexidades das funções internas
 *  descritas individualmente.
 */
const vector<Highway*>& extend_highways_kruskal(const vector<City> &cities,
                                          const vector<Highway> &existing) {
  static std::vector<Highway*> empty;
  size_t connectedCount = init_cities_groups(existing);
  if (connectedCount == cities.size() - 1) return empty;

  auto& candidates = gen_sorted_candidates(cities);
  return pick_highways(candidates, cities.size());
}

template<typename Comparator>
inline void gen_highways(Group& from_group,
                         Group& not_to_group,
                         const vector<City>& cities,
                         heap<Highway*, Comparator>* out) {
  static std::vector<const City*> to_cities(751);
  to_cities.clear();

  for (auto& c : cities) {
    if (c.group != from_group && c.group != not_to_group) {
      to_cities.push_back(&c);
    }
  }
  for (auto& from : cities) {
    if (from.group == from_group) {
      for (auto* to : to_cities) {
        buffer.emplace_back(&from, to);
        out->push(&buffer.back());
      }
    }
  }
}

const vector<Highway*>& extend_highways_prim(const vector<City> &cities,
                                             const vector<Highway> &existing) {
  static std::vector<Highway*> created(751);
  created.clear();

  size_t connectedCount = init_cities_groups(existing);
  if (connectedCount == cities.size() - 1) return created;

  static auto candidates = make_heap<Highway*>(buffer.capacity(),
                                               by_deref_rev<Highway*>);
  candidates.clear();
  buffer.clear();

  Group& main = cities.front().group;
  gen_highways(main, main, cities, &candidates);
  while (main.size() < cities.size()) {
    Highway* h = candidates.pop_copy();

    const City* to_add = main != h->city1->group ? h->city1 :
                         main != h->city2->group ? h->city2 :
                         nullptr;
    if (to_add) {
      gen_highways(to_add->group, main, cities, &candidates);
      main.merge(to_add->group);
      created.push_back(h);
    }
  }
  return created;
}

/**
 * Função auxiliar de leitura das cidades da entrada padrão.
 */
void read_cities(vector<City>* cities) {
  int num_cities;
  cities->clear();

  cin >> num_cities;
  for (int i = 1; i <= num_cities; i++) {
      cities->emplace_back(i);
      cin >> cities->back();
    }
}

/**
 * Função auxiliar de leitura das estradas da entrada padrão. Precisa-se
 * de uma referência também para o vetor de cidades visto que as estradas
 * usam ponteiros para essas estruturas para representar as cidades que
 * ligam.
 */
void read_highways(const vector<City>& cities, vector<Highway>* highways) {
  int num_highways, hc1, hc2;
  highways->clear();

  cin >> num_highways;
  while (num_highways-- > 0) {
      cin >> hc1 >> hc2;
      highways->emplace_back(&cities[hc1 - 1], &cities[hc2 - 1]);
    }
}

int main() {
  std::ios::sync_with_stdio(false);

  int num_cases;
  static vector<City> cities(751);
  static vector<Highway> existing_highways(1001);

  cin >> num_cases;
  while (num_cases-- > 0) {
    // Leitura de entrada
    read_cities(&cities);
    read_highways(cities, &existing_highways);

    // Cálculo dos resultados para o problema
    const auto& prim = extend_highways_prim(cities, existing_highways);

    auto& new_highways = prim;
    // Impressão dos resultados
    if (new_highways.empty()) {
      cout << "No new highways need" << std::endl;
    } else {
      for (auto h : new_highways) {
        cout << *h << std::endl;
      }
    }
    if (num_cases > 0) cout << std::endl;
  }
}
