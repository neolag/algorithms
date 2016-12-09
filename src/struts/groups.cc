
/**
 * Estrutura que representa um conjunto, utilizando ponteiros para
 * conjuntos "pais" quando ocorre uma união entre dois deles.
 *
 * Denominou-se de 'Group' no lugar de 'Set' para não se confundir
 * com as classes de conjunto na biblioteca padrão, de funcionalidade
 * fundamentalmente diferente. Se utilizará a denominação de grupo no
 * lugar de conjunto daqui pra frente.
 *
 * Apesar da implementação interna não ser simétrica (sempre haverá
 * um conjunto "pai" quando se faz uma união), a interface foi desenhada
 * para poder ser utilizada como se não houvesse diferença entre os
 * grupos em nenhum momento, o que de qualquer forma faz sentido em
 * relação tanto à eficiência quanto à semântica das operações.
 */
struct Group {
  /**
   * Compara se dois grupos são iguais, o que significa que
   * já foram unidos em algum momento. Internamente acha e
   * compara a raiz dos dois grupos recebidos.
   */
  inline bool operator==(Group& o) {
    return root_group() == o.root_group();
  }
  inline bool operator!=(Group& o) { return !(*this == o); }

  Group(const Group&) = delete;
  Group(Group&&) = default;
  Group() = default;

  inline int size() {
    return root_group()->size_;
  }

  inline void merge(Group& o) {
    auto r = root_group(), othr = o.root_group();
    othr->parent_ = r;
    r->size_ += othr->size_;
  }

  /**
   * Faz a união entre dois grupos diferentes. Essa operação
   * não é simétrica, mas a eficiência e o resultado semântico
   * da execução são os mesmos independentemente da ordem dos
   * argumentos portanto manteve-se a interface simétrica.
   *
   * Com "resultado semântico" se quer dizer todas as relações
   * que serão criadas entre os grupos existentes e que podem
   * ser checadas com os operadores definidos acima.
   */
  static bool merge(Group& g1, Group& g2) {
    if (g1 == g2) return false;

    if (g1.size() >= g2.size()) {
      g1.merge(g2);
    } else {
      g2.merge(g1);
    }
    return true;
  }

  void reset() {
    root_group()->size_--;
    parent_ = this;
    size_ = 1;
  }

 private:
  // ponteiro pro grupo "pai"
  Group* parent_ = this;
  int size_ = 1;

  /**
   * Função auxiliar para achar o grupo raiz, otimiza a busca
   * colapsando o grupo pai de todos os grupos no "caminho"
   * até ele para o grupo raiz, de forma que a partir da
   * segunda chamada consecutiva (sem uniões) pode ser
   * executada em O(1). No limite ao infinito e fazendo-se
   * tantas uniões quanto se quiser, a média geral de
   * complexidade continua como O(1) devido à amortização
   * dos casos em que se terão grandes listas encadeadas de
   * grupos, visto que serão percorridas uma única vez e
   * colapsadas num grupo só com referências diretas para a
   * raiz.
   */
  inline Group* root_group() {
    if (parent_->parent_ == parent_) return parent_;
    return parent_ = parent_->root_group();
  }
};
