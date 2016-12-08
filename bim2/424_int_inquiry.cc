// UVA problem 424:
// https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=365

#include <iostream>

#include <vector>
#include <string>

/**
 * Função auxiliar para fazer a soma de dois dígitos e
 * adicionar o carry a uma terceira variável recebida por
 * ponteiro.
 */
void add_digits(uint8_t* a, uint8_t b, uint8_t* carry) {
  *a += b;
  if (*a > 9) {
    ++*carry;
    *a %= 10;
  }
}

/**
 * Implementação própria para um número inteiro indefinidamente
 * grande. Faz o armazenamento do número através de um dígito
 * por byte (uint8_t), o que não é o mais eficiente possível mas
 * é o bastante para os limites de entrada no problema em questão.
 */
struct VeryLongInteger {
    std::vector<uint8_t> digits;

    VeryLongInteger() : digits(1) {}

    explicit VeryLongInteger(const std::string& s) : digits(s.size()) {
      for (int i = 0; i < s.size(); i++) {
        digits[i] = (uint8_t) (s[s.size() - i - 1] - '0');
      }
    }

    /**
     * Operador de adição, implementação principal para o problema resolvido.
     * Faz a soma com o outro "InteiroMuitoGrande" dígito por dígito,
     * salvando e adicionando o carry para cada um de modo que a operação
     * completa funcione como esperado.
     */
    VeryLongInteger& operator+=(const VeryLongInteger& other) {
      uint8_t carry = 0;
      for (int i = 0; i < other.digits.size(); i++) {
        if (carry) {
          carry = 0;
          add_digits(&digit(i), 1, &carry);
        }
        add_digits(&digit(i), other.digits[i], &carry);
      }

      // Continuar a soma enquanto houver carry (ie. todos os últimos digitos 9)
      for (int i = other.digits.size(); carry; i++) {
        carry = 0;
        add_digits(&digit(i), 1, &carry);
      }
      return *this;
    }

    /**
     * Função auxiliar usada internamente para pegar o valor
     * de um certo dígito, fazendo o reajuste do tamanho do
     * vector caso necessário.
     */
    uint8_t& digit(int i) {
      if (i >= digits.size()) {
        digits.resize(i + 1);
      }
      return digits[i];
    }
};

// Função auxiliar para escrever VeryLongInteger em stdout
std::ostream& operator<<(std::ostream& s, const VeryLongInteger& big) {
  for (int i = big.digits.size() - 1; i >= 0; i--) {
    s << (char)('0' + big.digits[i]);
  }
  return s;
}

int main() {
  VeryLongInteger sum;
  std::string str;
  while (std::cin >> std::ws >> str && str != "0") {
    sum += VeryLongInteger(str);
  }
  std::cout << sum << std::endl;
  return 0;
}
