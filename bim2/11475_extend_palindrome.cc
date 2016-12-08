// UVA Problem 11.475:
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2470

#include <iostream>

#include <string>
#include <vector>

/**
 * Função para construir a tabela de "backtracking" do algoritmo
 * de KMP. Feita com template de modo que possa ser implementada
 * de modo genérico para uma string normal, mas que possa também
 * receber um iterador *reverso* de uma string para a adaptação
 * do algoritmo KMP para encontrar o maior palíndrome.
 */
template<typename String>
std::vector<int> build_kmp_table(String str, size_t size) {
  std::vector<int> longestPrefixes(size);
  longestPrefixes[0] = -1;

  // Algoritmo padrão para geração da tabela KMP. Valor na tabela
  // para posição i guardará o maior prefixo da string encontrado
  // logo antes do caractere nessa posição (exclusivo).
  int currentPrefixSize = 0;
  for (int i = 1; i < size; i++) {
    longestPrefixes[i] = currentPrefixSize;
    if (str[i] == str[currentPrefixSize]) {
      currentPrefixSize++;
    } else {
      // Caso o caractere atual não seja igual ao caractere necessário
      // para estender o maior prefixo encontrado atualmente, verificar
      // para prefixos progressivamente menores acessando a própria
      // tabela sendo criada.
      while (currentPrefixSize > 0) {
        currentPrefixSize = longestPrefixes[currentPrefixSize];
        if (str[i] == str[currentPrefixSize]) {
          currentPrefixSize++;
          break;
        }
      }
    }
  }
  return longestPrefixes;
}
// Exemplo de como a função seria utilizada para uma string normal.
std::vector<int> build_kmp_table(const std::string& str) {
  return build_kmp_table(str.begin(), str.size());
}

/**
 * Calcula o tamanho do maior palíndrome possível que seja sufixo
 * da string recebida. Utiliza o algoritmo KMP com uma condição
 * de parada modificada, procurando pela própria string recebida
 * em ordem reversa em si mesma.
 */
int trailing_palind_size(const std::string& str) {
  // rbegin retorna um iterador reverso que pode ser utilizado
  // para acessar a string de trás pra frente com o operador []
  auto rev_str = str.rbegin();
  // Só precisa-se da tabela KMP para metade da string
  auto rev_kmp = build_kmp_table(rev_str, (str.size() + 1) / 2);

  int start = 0, i = 0;
  while (start < str.size()) {
    if (str[start + i] == rev_str[i]) {
      i++;
    } else {
      start += i - rev_kmp[i];
      if (i > 0) i = rev_kmp[i];
    }
    // O segundo termo nessa comparação é o índice resultante
    // ao fazermos o acesso pelo Iterador reverso. Note que
    // poderá ocorrer uma ultrapassagem "por muito" caso a
    // atualização de start/i tenha ocorrido no else acima. A
    // posição start continuaria sendo o começo do palíndrome
    // porém, apenas o índice i que estaria mais avançado.
    if (start + i >= (int) str.size() - 1 - i) {
      return str.size() - start;
    }
  }
  return 1;
}

/**
 * Função auxiliar para imprimir todos os elementos encontrados entre dois
 * iteradores para uma certa stream.
 */
template<typename Iterator>
std::ostream& write_all(std::ostream& s, Iterator begin, const Iterator& end) {
  while (begin != end) {
    s << *begin;
    ++begin;
  }
  return s;
}

int main() {
  using namespace std;
  std::string line = "";

  while (std::getline(cin, line) && !cin.eof()) {
    cout << line;
    int palindSize = trailing_palind_size(line);
    // Usando rbegin/rend os caracteres da string serão impressos em ordem contrária
    write_all(cout, line.rbegin() + palindSize, line.rend());
    cout << std::endl;
  }
  return 0;
}
