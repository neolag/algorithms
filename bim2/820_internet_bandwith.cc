// UVA Problem 820:
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=761

#include <iostream>

#include <algorithm>
#include <queue>
#include <climits>

using std::cin;
using std::cout;
using std::min;

struct Node;

/**
 * Classe para representar uma conexão entre dois nós. É
 * compartilhada entre os dois nós da conexão de forma a
 * manter a mesma banda nos dois sentidos do fluxo. Guarda
 * também ponteiros para os dois nós ligados pela conexão.
 */
struct Link {
    Node* nodes[2];
    int capacityLeft;

    void init(Node *n1, Node* n2, int capacity) {
      nodes[0] = n1;
      nodes[1] = n2;
      capacityLeft = capacity;
    }
};

/**
 * Classe para representar os nós propriamente ditos.
 */
struct Node {
    std::vector<Link*> links;

    // Utilidade para a BFS, usada para recuperar
    // o caminho até o nó atual.
    mutable Link* previousLink;

    /**
     * Obtem o nó anterior a partir do campo 'previousLink'.
     */
    Node* previousNode() const {
      if (!previousLink) return nullptr;
      return this->otherNode(previousLink);
    }

    /**
     * Utilidade para pegar o outro nó ao qual um devido Link
     * conecta o nó atual.
     */
    Node* otherNode(const Link* l) const {
      if (this == l->nodes[0]) return l->nodes[1];
      if (this == l->nodes[1]) return l->nodes[0];
      return nullptr;
    }

    /**
     * Utilidade para buscar um Link existente para um dado nó.
     */
    Link* existingLinkTo(const Node *n) const {
      for (Link* l : links) {
        if (otherNode(l) == n) {
          return l;
        }
      }
      return nullptr;
    }
};

/**
 * Limpa o campo previousLink para todos os nós dados. Necessário
 * antes da execução de uma BFS.
 */
void clear_paths(Node *begin, Node *end) {
  for (Node* curr = begin; curr != end; curr++) {
    curr->previousLink = nullptr;
  }
}

/**
 * Realiza uma busca em largura a partir de source até target
 * utilizando apenas conexões com alguma capacidade restante,
 * e retorna se algum caminho foi encontrado.
 *
 * Guarda o caminho percorrido até cada um dos nós a partir do
 * campo previousLink na classe Node. Esse campo será utilizado
 * posteriormente então para recuperar o caminho percorrido e
 * determinar o fluxo máximo do mesmo, atualizando também a
 * capacidade da rede. Note que o campo previousLink é
 * utilizado também para marcar o nó como "encontrado" pela BFS.
 */
bool bfs(const Node* source, const Node* target) {
  // Usamos uma fila estática para evitar criações e destruções
  // de arrays grandes a cada chamada. Como o algoritmo executa
  // em uma thread só, não há problema nisso.
  static std::vector<const Node*> queue(110);
  queue.clear();

  queue.push_back(source);
  for (int i = 0; i < queue.size(); i++) {
    const Node* curr = queue[i];
    // Itera sobre todas as conexões do nó atual
    for (Link* l : curr->links) {
      const Node* other = curr->otherNode(l);
      // Desconsideramos conexões com capacityLeft == 0
      if (l->capacityLeft > 0 && other->previousLink == nullptr && other != source) {
        other->previousLink = l;
        if (other == target) {
          return true;
        }
        queue.push_back(other);
      }
    }
  }
  return false;
}

/**
 * Calcula o fluxo máximo para o caminho previamente calculado pela BFS de
 * source até target.
 */
int max_path_flow(const Node *source, const Node *target) {
  int max_flow = INT_MAX;
  for (const Node* curr = target; curr != source; curr = curr->previousNode()) {
    max_flow = min(max_flow, curr->previousLink->capacityLeft);
  }
  return max_flow;
}

/**
 * Atualiza a capacidade da rede no caminho previamente calculado pela BFS
 * de source até target.
 */
void update_path_capacity(Node* source, Node* target, int flow) {
  for (Node* curr = target; curr != source; curr = curr->previousNode()) {
    curr->previousLink->capacityLeft -= flow;
  }
}

int main() {
  Node nodes[110];
  Link links_buffer[11010];
  size_t numNodes, sourceIdx, targetIdx, numLinksToRead;

  for (int networkId = 1; cin >> numNodes && numNodes > 0; networkId++) {
    // Reinicialização das estruturas
    for (int i = 0 ; i < numNodes; i++) {
      nodes[i].links.clear();
    }
    cin >> sourceIdx >> targetIdx >> numLinksToRead;

    // Leitura das conexões. Como os links são compartilhados
    // entre os nós, utilizamos um buffer com o número máximo
    // de links que serão necessários de forma a evitar criação
    // e destruição sucessiva de objetos.
    int linkCount = 0;
    for (int i = 0; i < numLinksToRead; i++) {
      int n1, n2, cap;
      cin >> n1 >> n2 >> cap;

      // Faz-se ainda um colapso entre múltiplas conexões entre
      // os mesmos nós, considerando que seja apenas uma conexão
      // com a soma das capacidades de todas que foram definidas.
      Link* link = nodes[n1 - 1].existingLinkTo(&nodes[n2 - 1]);
      if (link) {
        link->capacityLeft += cap;
      } else {
        link = &links_buffer[linkCount++];
        link->init(&nodes[n1 - 1], &nodes[n2 - 1], cap);
        nodes[n1 - 1].links.push_back(link);
        nodes[n2 - 1].links.push_back(link);
      }
    }

    // Algoritmo de Edmonds–Karp: Executa sucessivamente uma BFS
    // entre os nós de origem e destino, **considerando apenas as
    // conexões entre os mesmos com alguma capacidade livre**. Em
    // seguida, utiliza a capacidade máxima do caminho encontrado,
    // atualizando a capacidade restante das conexões entre os nós.
    // Faz isso até que não sejam mais encontrados caminhos entre
    // os nós de origem e destino.
    Node* source = &nodes[sourceIdx - 1];
    Node* target = &nodes[targetIdx - 1];
    int totalFlow = 0;
    while (true) {
      clear_paths(nodes, nodes + numNodes);
      if (!bfs(source, target)) break;

      int pathFlow = max_path_flow(source, target);
      update_path_capacity(source, target, pathFlow);
      totalFlow += pathFlow;
    }

    cout << "Network " << networkId << std::endl;
    cout << "The bandwidth is " << totalFlow << "." << std::endl << std::endl;
  }
  return 0;
}
