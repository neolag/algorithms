// UVA Problem 374:
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=310#include <iostream>

#include <iostream>

#include <vector>
#include <cstdint>
#include <climits>

using std::vector;

 /**
  * Gerará todas as potências (quadráticas) de um certo número wrap
  * B módulo um outro número M até um expoente máximo max_e.
  * Esse expoente máximo será o limite das potências a serem
  * geradas de forma a evitar computação desnecessária.
  * Retorna um vector no qual cada posição i representa o
  * resultado da fórmula: B^(2^i) mod M
  * (B elevado à i-ésima potência de 2, módulo M)
  */
vector<int>& gen_powers(int B, int M, uint64_t max_e = ULLONG_MAX) {
  static vector<int> powers(65);

  powers[0] = B % M;
  for (int i = 1; max_e > 0; i++) {
    powers[i] = (powers[i - 1] * powers[i - 1]) % M;
    max_e /= 2; // otimizado pelo compilador
  }

  return powers;
}

// Função auxiliar para determinar se menor bit do número está setado (1).
bool is_first_bit_set(uint64_t v) {
  return (bool) (v & 0x01);
}

/**
 * Calcula a potência de um número B elevado a outro P
 * módulo M, utilizando a técnica de quadrados sucessivos.
 */
int mod_pow(int B, uint64_t P, int M) {
  auto& powers = gen_powers(B, M, P);

  // Pequena otimização: Utiliza como condição de parada também
  // se o valor de B^(2^idx) atual é diferente de 1, visto que
  // nesse caso todas as potências seguintes serão também 1, não
  // mudando  o valor do resultado.
  int res = 1, idx = 0;
  while (P > 0 && powers[idx] != 1) {
    if (is_first_bit_set(P)) {
      res = (res * powers[idx]) % M;
    }
    P /= 2;
    idx++;
  }
  return res;
}

int main() {
  using namespace std;
  int B, M;
  uint64_t P;

  while (cin >> B && !cin.eof()) {
    cin >> P >> M;
    cout << mod_pow(B, P, M)
         << endl;
  }
  return 0;
}
